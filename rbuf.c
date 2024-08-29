#include <stdlib.h>
#include <string.h>
#include "rbuf.h"

BOOL rbuf_init(RBUF *rbuf, int size)
{
    // ちょうどいいサイズにする。
    int s = size - 1;
    s |= s >> 1;
    s |= s >> 2;
    s |= s >> 4;
    s |= s >> 8;

    rbuf->buf_mask = s;
    s++;

    rbuf->buf = malloc(s);
    if(!rbuf->buf) {
        return FALSE;
    }

    rbuf->read_idx = 0;
    rbuf->write_idx = 0;

    return TRUE;
}

int rbuf_get_size(const RBUF *rbuf)
{
    return (rbuf->write_idx - rbuf->read_idx) & rbuf->buf_mask;
}

BOOL rbuf_add_data(RBUF *rbuf, const char data)
{
    if(((rbuf->write_idx + 1) & rbuf->buf_mask) == rbuf->read_idx) {
        return FALSE;
    }
    rbuf->buf[rbuf->write_idx] = data;
    rbuf->write_idx = (rbuf->write_idx + 1) & rbuf->buf_mask;

    return TRUE;
}

int rbuf_get_data(RBUF *rbuf)
{
    if(rbuf->read_idx == rbuf->write_idx) {
        return -1;
    }
    int ret = rbuf->buf[rbuf->read_idx];
    rbuf->read_idx = (rbuf->read_idx + 1) & rbuf->buf_mask;
    return ret & 0xff;
}

int rbuf_peek_data(RBUF *rbuf)
{
    if(rbuf->read_idx == rbuf->write_idx) {
        return -1;
    }
    int ret = rbuf->buf[rbuf->read_idx];
    return ret & 0xff;
}

int rbuf_unget(RBUF *rbuf)
{
    rbuf->read_idx = (rbuf->read_idx - 1) & rbuf->buf_mask;
}

int rbuf_read(RBUF *rbuf, char *buf, int size)
{
    int read_size = 0;

    for(int i = 0; i < size && rbuf_get_size(rbuf) != 0; i++) {
        buf[i] = rbuf_get_data(rbuf);
        read_size++;
    }
    return read_size;
}
