#include <stdio.h>
#include <string.h>
#include "iot.h"

/*
    https://github.com/hra1129/for_MSX0/tree/main/sample_program/002_device/2023_05_30_1st_update_version/basicn
    こちらのコードをCで書き直したものです。
*/
void iot_node_write(const char *node)
{
    outp(IOT_PORT, 0xe0);
    outp(IOT_PORT, 1);
    outp(IOT_PORT, 0x53);

    int len = strlen(node);
    outp(IOT_PORT, 0xc0);
    outp(IOT_PORT, len);

    for(int i = 0; i < len; i++) {
        outp(IOT_PORT, node[i]);
    }
    outp(IOT_PORT, 0);
    int r = inp(IOT_PORT);
}

void iot_puts(const char *node, const char *val)
{
    iot_node_write(node);

    outp(IOT_PORT, 0xe0);
    outp(IOT_PORT, 1);
    outp(IOT_PORT, 0x43);

    int len = strlen(val);
    outp(IOT_PORT, 0xc0);
    outp(IOT_PORT, len);
    for(int i = 0; i < len; i++) {
        outp(IOT_PORT, val[i]);
    }
    outp(IOT_PORT, 0);
    int r = inp(IOT_PORT);
}

void iot_puti(const char *node, const int val)
{
    iot_node_write(node);

    outp(IOT_PORT, 0xe0);
    outp(IOT_PORT, 1);
    outp(IOT_PORT, 0x41);

    outp(IOT_PORT, 0xc0);
    outp(IOT_PORT, 2);
    outp(IOT_PORT, val);
    outp(IOT_PORT, val >> 8);
}

short iot_geti(const char *node)
{
    iot_node_write(node);
///
    outp(IOT_PORT, 0xe0);
    outp(IOT_PORT, 1);
    outp(IOT_PORT, 1);
    outp(IOT_PORT, 0x80);

    short ret = 0;

    int r = inp(IOT_PORT);

    r = inp(IOT_PORT);
    ret = r;
    r = inp(IOT_PORT);
    ret |= (r << 8);

    return ret;
}

