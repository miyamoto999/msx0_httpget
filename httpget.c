#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include "bool.h"
#include "iot.h"

#define DATA_COUNT  3
#define NET_IF      "msx/me/if/NET0/"
#define NET_ADDR    "msx/me/if/NET0/conf/addr"
#define NET_PORT    "msx/me/if/NET0/conf/port"
#define NET_CONNECT "msx/me/if/NET0/connect"
#define NET_MSG     "msx/me/if/NET0/msg"

#define HEADER_CONTENT_LENGTH   "Content-Length"
#define HEADER_TRANSFER_ENCODING    "Transfer-Encoding"
#define HEADER_TRANSFER_ENCODING_CHUNKED   "chunked"

#define TIME_OUT    30
#define BUF_SIZE    256

#define MSXWORK_JIFFY   ((volatile uint16_t *)0xfc9e)
#define GET_JIFFY()     *MSXWORK_JIFFY

long heap = 0x8000;
static char *buf;

void net_connect(const char *hostname, int port)
{
    iot_puts(NET_ADDR, hostname);
    iot_puti(NET_PORT, port);
    iot_puti(NET_CONNECT, 1);
}

BOOL net_is_connected()
{
    return iot_geti(NET_CONNECT);
}

void net_discconect()
{
    iot_puti(NET_CONNECT, 0);
}

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


int main(int argc, char *argv[])
{
    char *datas[DATA_COUNT];
    long download_size = 0;
    BOOL chunked = FALSE;

    mallinit();
    sbrk(0x8000, 16 * 1024); 

    printf("HTTPGET Version 1.0\n");
    if(argc != 5) {
        fprintf(stderr, "Usage : httpget HOSTNAME PORT SRCPATH DESTNAME\n");
        return 1;
    }
    
    buf = malloc(BUF_SIZE);
    if(!buf) {
        fprintf(stderr, "Memory allocation error\n");
        return 1;
    }

    if(!iot_init()) {
        free(buf);
        fprintf(stderr, "Memory allocation error\n");
        return 1;
    }

    const char *hostname = argv[1];
    const int port = atoi(argv[2]);
    const char *src_path = argv[3];
    const char *destname = argv[4];

    printf("CONNECT %s:%d\n", hostname, port);
    time_t st = time(NULL);
    net_connect(hostname, port);

    while(!net_is_connected()) {
        if(time(NULL) - st >= TIME_OUT) {
            fprintf(stderr, "CONNECT TIMEOUT\n");
            return 1;
        }
    }
    printf("CONNECTED\n");
    sprintf(buf, "GET %s HTTP/1.1\x0d\x0a", src_path);
    iot_puts(NET_MSG, buf);
    sprintf(buf, "HOST: %s:%d\x0d\x0a", hostname, port);
    iot_puts(NET_MSG, buf);
    iot_puts(NET_MSG, "CONNECTION: close\x0d\x0a");
    iot_puts(NET_MSG, "\x0d\x0a");

    char *data = iot_readline(NET_MSG, NET_CONNECT);
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
        char *data = iot_readline(NET_MSG, NET_CONNECT);
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
        if(!net_is_connected() && strlen(data) == 0) {
            fprintf(stderr, "Network ERROR???\n");
            return 1;
        }
    }
    if(download_size == 0 && !chunked) {
        fprintf(stderr, "Not %s header.", HEADER_CONTENT_LENGTH);
        return 1;
    }

    FILE *fp = fopen(destname, "wb");
    if(!fp) {
        fprintf(stderr, "Write file create ERROR\n");
        return 1;
    }

    long data_size = 0;
    if(chunked) {
        download_size = 1;
    }
    uint16_t prev_jiffy = 0;
    while(data_size < download_size) {
        int read_size = BUF_SIZE;
        long chunk_size = BUF_SIZE;
        if(chunked) {
            char *str = iot_readline(NET_MSG, NET_CONNECT);
            chunk_size = strtol(str, NULL, 16);
            // printf("str = %s\n", str);
            // printf("chunk_size = %ld\n", chunk_size);
            if(chunk_size == 0) {
                iot_readline(NET_MSG, NET_CONNECT);
                break;
            }
        }
        int len;
        while(chunk_size > 0) {
            if(chunk_size < BUF_SIZE) {
                read_size = chunk_size;
            } else {
                read_size = BUF_SIZE;
            }
            len = iot_read(NET_MSG, buf, read_size);
            if(len != 0) {
                size_t ret = fwrite(buf, 1, len, fp);
                if(ret < len && errno != 0) {
                    fprintf(stderr, "\nFile Write ERROR\n");
                    fclose(fp);
                    return 1;
                }
            } else if(len == 0 && !net_is_connected()) {
                fprintf(stderr, "Network ERROR???\n");
                fclose(fp);
                return 1;
            }
            data_size += len;
            if((GET_JIFFY() - prev_jiffy) >= 30) {
                prev_jiffy = GET_JIFFY();
                disp_progreass(chunked, destname, data_size, download_size);
            }
            if(chunked) {
                chunk_size -= len;
                download_size = data_size + 1;
            } else {
                break;
            }
        }
        if(chunked) {
            iot_readline(NET_MSG, NET_CONNECT);
        }
    }
    disp_progreass(chunked, destname, data_size, download_size);
    fprintf(stderr, "\n");
    if(fclose(fp) == EOF) {
        fprintf(stderr, "File Write ERROR\n");
    } else {
        printf("Commplited.\n");
    }

    return 0;
}