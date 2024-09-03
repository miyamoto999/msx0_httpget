#include <stdlib.h>
#include <time.h>
#include "bool.h"
#include "iot.h"
#include "net.h"

#define BUF_SIZE    256
static char *retbuf;

BOOL net_init()
{
    retbuf = malloc(BUF_SIZE);
    if(!retbuf) {
        return FALSE;
    }
    return TRUE;
}

BOOL net_connect(const char *hostname, int port, int time_out)
{
    time_t st = time(NULL);

    iot_puts(NET_ADDR, hostname);
    iot_puti(NET_PORT, port);
    iot_puti(NET_CONNECT, 1);

    while(!net_is_connected()) {
        if(time(NULL) - st >= time_out) {
            return FALSE;
        }
    }
    return TRUE;
}

static int read_char(RBUF *rbuf, const char *node)
{
    char data;
    int size;

    size = iot_read(rbuf, node, &data, 1);
    if(size == 0) {
        return -1;
    }
    return data & 0xff;
}

char *net_readline(RBUF *rbuf, const char *node, const char *node_connect)
{
    int size, idx;
    BOOL crflag = FALSE;

    idx = 0;

    while(1)
    {
        BOOL is_connected = iot_geti(node_connect);
        int ch = read_char(rbuf, node);
        if(ch == -1 && is_connected) {
            continue;
        } else if(ch == -1) {
            break;
        } else if(ch == 0x0a) {
            if(idx > 0 && retbuf[idx - 1] == '\x0d') {
                retbuf[idx - 1] = 0;
            } else {
                retbuf[idx] = 0;
                idx++;
            }
            crflag = TRUE;
            break;
        } else {
            retbuf[idx] = ch;
            idx++;
        }
    }

    if(!crflag) {
        retbuf[idx] = 0;
    }
    return retbuf;
}
