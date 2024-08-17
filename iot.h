#ifndef __IOT_H__
#define __IOT_H__

#include "bool.h"

//#define IOT_PORT    8
#define IOT_PORT    0x58

BOOL iot_init();
void iot_puts(const char *node, const char *val);
void iot_puti(const char *node, const int val);
short iot_geti(const char *node);
// char *iot_gets(const char *node);
void iot_node_write(const char *node);

int iot_read(const char *node, char *buf, int size);
char *iot_readline(const char *node, const char *node_connect);

int rbuf_read(char *buf, int size);

#endif