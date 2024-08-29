#include <stdio.h>
#include <stdlib.h>
#include "bool.h"
#include "iot.h"
#include "rbuf.h"

#define BUF_SIZE    256
static char *retbuf;

BOOL iot_init()
{
    retbuf = malloc(BUF_SIZE);
    if(!retbuf) {
        return FALSE;
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

char *iot_readline(RBUF *rbuf, const char *node, const char *node_connect)
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

int iot_read(RBUF *rbuf, const char *node, char *buf, int size)
{
    int ret = 0;

    if(rbuf_get_size(rbuf) == 0) {
        iot_node_write(node);

        outp(IOT_PORT1, 0xe0);
        outp(IOT_PORT1, 1);
        outp(IOT_PORT1, 3);
        outp(IOT_PORT1, 0x80);

        int len = inp(IOT_PORT1);       // 試してみたかんじ、64バイト以上にはならないようだ。
        for(int i = 0; i < len; i++) {
            if(i >= size) {
                rbuf_add_data(rbuf, inp(IOT_PORT1));
            } else {
                buf[i] = inp(IOT_PORT1);
                ret++;
            }
        }
    } else {
        for(int i = 0; i < size && rbuf_get_size(rbuf) != 0; i++) {
            buf[i] = rbuf_get_data(rbuf);
            ret++;
        }
    }

    return ret;
}
