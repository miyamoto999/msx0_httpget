#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include "bool.h"
#include "iot.h"
#include "msxdos.h"
#include "buf_file.h"
#include "net.h"
#include "rbuf.h"

#define VERSION     "1.5 pre"

#define DATA_COUNT  3

#define HEADER_CONTENT_LENGTH   "Content-Length"
#define HEADER_TRANSFER_ENCODING    "Transfer-Encoding"
#define HEADER_TRANSFER_ENCODING_CHUNKED   "chunked"

#define TIME_OUT    30
#define BUF_SIZE    256

#define MSXWORK_JIFFY   ((volatile uint16_t *)0xfc9e)
#define GET_JIFFY()     *MSXWORK_JIFFY

#define MSXWORK_CSRSW   ((uint8_t *)0xfca9)

#define FILE_BUF_SIZE   2048
#define RING_BUF_SIZE   1024

long heap;
static char *buf;

BOOL abort_flag = FALSE;
BOOL write_err_flag = FALSE;
BOOL net_err_flag = FALSE;

static void spliit_http_status(char *status, char *datas[])
{
    int len = strlen(status);
    int j = 0;

    for(int i = 0; i < 2; i++) {
        datas[i] = &status[j];
        for(;j < len; j++) {
            if(status[j] == ' ') {
                status[j] = 0;
                j++;
                break;
            }
        }
    }
    datas[2] = &status[j];
}

static void spliit_header(char *header, char *datas[])
{
    int len = strlen(header);
    int i = 0, j = 0;

    datas[i] = &header[j];
    i++;
    for(;j < len; j++) {
        if(header[j] == ':') {
            header[j] = 0;
            j++;
            break;
        }
    }
    // スペースをスキップ
    for(;j < len; j++) {
        if(header[j] != ' ') {
            datas[i] = &header[j];
            break;
        }
    }
    // 後ろのスペースを念のため削除
    len = strlen(datas[i]);
    for(j = len - 1; j >= 0; j--) {
        if(datas[i][j] == ' ') {
            datas[i][j] = 0;
        } else {
            break;
        }
    }
}

void disp_progreass(BOOL chunked, const char *filename, long data_size, long total_size)
{
    if(chunked) {
        fprintf(stderr, "%s (%ld)\x0d", filename, data_size);
    } else {
        fprintf(stderr, "%s (%ld/%ld)\x0d", filename, data_size, total_size);
    }
}

#ifdef __MSXDOS_MSXDOS2
BOOL abort_routine(uint8_t err1, uint8_t err2)
{
    if(err1 == ERR_CTRLC || err1 == ERR_STOP) {
        abort_flag = TRUE;
        return TRUE;
    }
    return FALSE;
}
#endif

int main(int argc, char *argv[])
{
    char *datas[DATA_COUNT];
    long download_size = 0;
    BOOL chunked = FALSE;
    RBUF *rbuf;
    uint16_t kernel_var, dos_ver;
    char *disp_fname = NULL;

    mallinit();
    sbrk(0x8000, 16 * 1024);

#ifdef __MSXDOS_MSXDOS1
    int cmd_len = *((uint8_t *)0x0080);
    *((uint8_t *)0x0081 + cmd_len) = 0;
#endif

#ifdef __MSXDOS_MSXDOS1
    printf("HTTPGET1 Version %s(DOS1 Version)\n", VERSION);
#else
    printf("HTTPGET Version %s\n", VERSION);
#endif

#ifdef __MSXDOS_MSXDOS2
    dos2_dosver(&kernel_var, &dos_ver);
    if((kernel_var >> 8) < 2) {
        fprintf(stderr, "This command is DOS2 Version.");
        return 1;
    }
#endif

    if(argc != 4 && argc != 5) {
        fprintf(stderr, "Usage : httpget HOSTNAME PORT SRCPATH [DESTNAME]\n");
        return 1;
    }
    
    buf = malloc(BUF_SIZE);
    if(!buf) {
        fprintf(stderr, "Memory allocation error\n");
        return 1;
    }

    if(!net_init()) {
        fprintf(stderr, "Memory allocation error\n");
        return 1;
    }

    rbuf = rbuf_create(RING_BUF_SIZE);
    if(!rbuf) {
        fprintf(stderr, "Memory allocation error\n");
        return 1;
    }

    disp_fname = malloc(FCB_NAME_SIZE + FCB_EXT_SIZE + 2);
    if(!disp_fname) {
        fprintf(stderr, "Memory allocation error\n");
        return 1;
    }
    disp_fname[FCB_NAME_SIZE + FCB_EXT_SIZE + 1] = 0;
    
#ifdef __MSXDOS_MSXDOS2
    dos2_defab(abort_routine);
#endif

    const char *hostname = argv[1];
    const int port = atoi(argv[2]);
    char *src_path = argv[3];
    char *destname = argv[4];

    if(src_path[0] != '/') {
        int len = strlen(src_path);
        char *tmp = malloc(len + 2);
        if(!tmp) {
            fprintf(stderr, "Memory allocation error\n");
            return 1;
        }
        tmp[0] = '/';
        strcpy(&tmp[1], src_path);
        src_path = tmp;
    }
    
    if(argc == 4) {
        int len = strlen(src_path);
        int i;
        for(i = len -1; i >= 0; i--) {
            if(src_path[i] == '/') {
                break;
            }
        }
        destname = &src_path[i+1];
    }

#ifdef __MSXDOS_MSXDOS2
    /* パスを解析 */
    char *last_str, *start_filename;
    uint8_t analysis_flag, drv;
    if(dos2_parse(destname, 0, &last_str, &start_filename, &analysis_flag, &drv) != 0) {
        fprintf(stderr, "Save Filename ERROR\n");
        return 1;
    }
    /* ファイル名の解析 */
    dos2_pfile(start_filename, disp_fname, &last_str, &analysis_flag);
    char *p = &disp_fname[FCB_NAME_SIZE + FCB_EXT_SIZE - 1];
    for(int i = 0; i < FCB_EXT_SIZE; i++) {
        *(p + 1) = *p;
        p--;
    }
    *(p + 1) = '.';
#endif

    printf("CONNECT %s:%d\n", hostname, port);
    int ret = net_connect(hostname, port, TIME_OUT);
    if(ret == NET_ERR_TIMEOUT) {
        fprintf(stderr, "CONNECT TIMEOUT.\n");
        return 1;
    } else if(ret == NET_ERR_ABORT) {
        fprintf(stderr, "Abort.\n");
        return 1;
    }

    time_t start_time = time(NULL);
    printf("CONNECTED\n");
    sprintf(buf, "GET %s HTTP/1.1\x0d\x0a", src_path);
    net_write_str(buf);
    sprintf(buf, "HOST: %s:%d\x0d\x0a", hostname, port);
    net_write_str(buf);
    net_write_str("CONNECTION: close\x0d\x0a");
    net_write_str("\x0d\x0a");

    char *data = net_readline(rbuf);
    if(strncmp(data, "HTTP", 4) != 0) {
        fprintf(stderr, "Not Recv HTTP Status\n");
        net_discconect();
        return 1;
    }
    spliit_http_status(data, datas);
    int status = atoi(datas[1]);
    if(status != 200) {
        fprintf(stderr, "HTTP status code error:%d %s\n", status, datas[2]);
        return 1;
    }
    while(1) {
        BOOL is_connedted = net_is_connected();
        char *data = net_readline(rbuf);
        int len = strlen(data);
        if(len == 0) {
            break;
        }
        spliit_header(data, datas);
        if(strcmp(datas[0], HEADER_CONTENT_LENGTH) == 0) {
            download_size = atol(datas[1]);
        } else if(strcmp(datas[0], HEADER_TRANSFER_ENCODING) == 0) {
            if(strcmp(datas[1], HEADER_TRANSFER_ENCODING_CHUNKED) == 0) {
                chunked = TRUE;
            } else {
                fprintf(stderr, "Not supported %s:%s\n", HEADER_TRANSFER_ENCODING, HEADER_TRANSFER_ENCODING_CHUNKED);
                return 1;
            }
        }
        if(!is_connedted && strlen(data) == 0) {
            fprintf(stderr, "Network ERROR???\n");
            return 1;
        }
    }
    if(download_size == 0 && !chunked) {
        fprintf(stderr, "Not %s header.", HEADER_CONTENT_LENGTH);
        return 1;
    }

    BFILE *bfp = bfile_create(destname, FILE_BUF_SIZE);
    if(!bfp) {
        fprintf(stderr, "Write file create ERROR\n");
        return 1;
    }
#ifdef __MSXDOS_MSXDOS1
    memcpy(disp_fname, bfp->fcb.name, FCB_NAME_SIZE);
    disp_fname[FCB_NAME_SIZE] = '.';
    memcpy(&disp_fname[FCB_NAME_SIZE + 1], bfp->fcb.ext, FCB_EXT_SIZE);
#endif

    long data_size = 0;
    uint16_t prev_jiffy = 0;
#ifdef __MSXDOS_MSXDOS1
    *MSXWORK_CSRSW = 0;
#endif
    while(chunked || data_size < download_size) {
        int read_size = BUF_SIZE;
        long chunk_size = BUF_SIZE;
        if(chunked) {
            char *str = net_readline(rbuf);
            chunk_size = strtol(str, NULL, 16);
            // printf("str = %s\n", str);
            // printf("chunk_size = %ld\n", chunk_size);
            if(chunk_size == 0) {
                net_readline(rbuf);
                break;
            }
        }
        int len;
        while(chunk_size > 0) {
            if(dos1_dirio(0xff) == 0x03) {
                abort_flag = TRUE;
            }
            if(abort_flag) {
                net_discconect();
                break;
            }
            if(chunk_size < BUF_SIZE) {
                read_size = chunk_size;
            } else {
                read_size = BUF_SIZE;
            }
            BOOL is_connected = net_is_connected();
            len = iot_read(rbuf, NET_MSG, buf, read_size);
            if(len != 0) {
                uint16_t ret = bfile_write(bfp, buf, len);
                if(ret <= 0) {
                    write_err_flag = TRUE;
                    break;
                }
            } else if(len == 0 && !is_connected) {
                net_err_flag = TRUE;
                break;
            }
            data_size += len;
            if((GET_JIFFY() - prev_jiffy) >= 30) {
                prev_jiffy = GET_JIFFY();
                disp_progreass(chunked, disp_fname, data_size, download_size);
            }
            if(chunked) {
                chunk_size -= len;
            } else {
                break;
            }
        }
        if(abort_flag || write_err_flag || net_err_flag) {
            break;
        }
        if(chunked) {
            net_readline(rbuf);
        }
    }
    disp_progreass(chunked, disp_fname, data_size, download_size);
#ifdef __MSXDOS_MSXDOS1
    *MSXWORK_CSRSW = 1;
#endif
    printf("\n");
    ret = bfile_close(bfp);
    long elapsed_time = time(NULL) - start_time;
    if(ret != 0) {
        fprintf(stderr, "File Write ERROR\n");
    } else {
        if(net_err_flag) {
            fprintf(stderr, "Network ERROR???\n");
        } else if(write_err_flag) {
            fprintf(stderr, "Write Error\n");
        } else if(abort_flag) {
            printf("Abort.\n");
        } else {
            int sec = elapsed_time % 60;
            int min = elapsed_time / 60;
            printf("Commplited.(%02d:%02d)\n", min, sec);
        }
    }

    return 0;
}