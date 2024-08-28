#include <stdio.h>
#include <stdlib.h>
#include "bool.h"
#include "iot.h"

#define RING_BUF_SIZE   0x400
#define RING_BUF_MASK   (RING_BUF_SIZE - 1)
static char *rbuf_buf;
static int rbuf_read_idx = 0;
static int rbuf_write_idx = 0;

#define BUF_SIZE    256
static char *buf;
static char *retbuf;

BOOL iot_init()
{
    rbuf_buf = malloc(RING_BUF_SIZE);
    if(!rbuf_buf) {
        return FALSE;
    }
    buf = malloc(BUF_SIZE);
    if(!buf) {
        free(rbuf_buf);
        return FALSE;
    }
    retbuf = malloc(BUF_SIZE);
    if(!retbuf) {
        free(rbuf_buf);
        free(buf);
        return FALSE;
    }
    return TRUE;
}

static int rbuf_get_size()
{
    return (rbuf_write_idx - rbuf_read_idx) & RING_BUF_MASK;
}

static BOOL rbuf_add_data(const char data)
{
    if(((rbuf_write_idx + 1) & RING_BUF_MASK) == rbuf_read_idx) {
        return FALSE;
    }
    rbuf_buf[rbuf_write_idx] = data;
    rbuf_write_idx = (rbuf_write_idx + 1) & RING_BUF_MASK;
    return TRUE;
}

static int rbuf_get_data() 
{
    if(rbuf_read_idx == rbuf_write_idx) {
        return -1;
    }
    int ret = rbuf_buf[rbuf_read_idx];
    rbuf_read_idx = (rbuf_read_idx + 1) & RING_BUF_MASK;
    return ret & 0xff;
}

static int rbuf_peek_data()
{
    if(rbuf_read_idx == rbuf_write_idx) {
        return -1;
    }
    int ret = rbuf_buf[rbuf_read_idx];
    return ret & 0xff;
}

static void rbuf_unget()
{
    rbuf_read_idx = (rbuf_read_idx - 1) & RING_BUF_MASK;
}

static int read_char(const char *node)
{
    char data;
    int size;

    size = iot_read(node, &data, 1);
    if(size == 0) {
        return -1;
    }
    return data & 0xff;
}

int rbuf_read(char *buf, int size)
{
    int read_size = 0;

    for(int i = 0; i < size && rbuf_get_size() != 0; i++) {
        buf[i] = rbuf_get_data();
        read_size++;
    }
    return read_size;
}

char *iot_readline(const char *node, const char *node_connect)
{
    int size, idx;
    BOOL crflag = FALSE;

    idx = 0;

    while(1)
    {
        int ch = read_char(node);
        if(ch == -1 && iot_geti(node_connect) == 1) {
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

int iot_read(const char *node, char *buf, int size)
{
    int ret = 0;

    if(rbuf_get_size() == 0) {
        iot_node_write(node);

        outp(IOT_PORT1, 0xe0);
        outp(IOT_PORT1, 1);
        outp(IOT_PORT1, 3);
        outp(IOT_PORT1, 0x80);

        int len = inp(IOT_PORT1);       // 試してみたかんじ、64バイト以上にはならないようだ。
        for(int i = 0; i < len; i++) {
            if(i >= size) {
                rbuf_add_data(inp(IOT_PORT1));
            } else {
                buf[i] = inp(IOT_PORT1);
                ret++;
            }
        }
    } else {
        for(int i = 0; i < size && rbuf_get_size() != 0; i++) {
            buf[i] = rbuf_get_data();
            ret++;
        }
    }

    return ret;
}
