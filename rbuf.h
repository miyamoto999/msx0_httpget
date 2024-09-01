#ifndef __RBUF_H__
#define __RBUF_H__

#include "bool.h"

typedef struct _rbuf {
    char *buf;
    int read_idx;
    int write_idx;
    int buf_mask;
} RBUF;

RBUF *rbuf_create(int size);
int rbuf_get_size(const RBUF *rbuf);
BOOL rbuf_add_data(RBUF *rbuf, const char data);
int rbuf_get_data(RBUF *rbuf);
int rbuf_peek_data(RBUF *rbuf);
int rbuf_unget(RBUF *rbuf);
int rbuf_read(RBUF *rbuf, char *buf, int size);
void rbuf_delete(RBUF *rbuf);

#endif