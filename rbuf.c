#include <stdlib.h>
#include <string.h>
#include "rbuf.h"

RBUF *rbuf_create(int size)
{
    RBUF *rbuf;

    rbuf = malloc(sizeof(RBUF));
    if(!rbuf) {
        return NULL;
    }
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
        free(rbuf);
        return NULL;
    }

    rbuf->read_idx = 0;
    rbuf->write_idx = 0;

    return rbuf;
}

void rbuf_delete(RBUF *rbuf)
{
    if(rbuf->buf) {
        free(rbuf->buf);
        rbuf->buf = NULL;
    }
    free(rbuf);
}

#if 0
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
#endif
