#ifndef __IOT_H__
#define __IOT_H__

#include "bool.h"
#include "rbuf.h"

//#define IOT_PORT    8
#define IOT_PORT1    0x58
#define IOT_PORT2    0x57

void iot_puts(const char *node, const char *val);
void iot_puti(const char *node, const int val);
short iot_geti(const char *node);
// char *iot_gets(const char *node);
void iot_node_write(const char *node);

int iot_read(RBUF *rbuf, const char *node, char *buf, int size);

int iot_read_1(RBUF *rbuf, const char *node, char *buf, int size);

#endif