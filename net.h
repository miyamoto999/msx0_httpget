#ifndef __NET_H__
#define __NET_H__

#include "bool.h"

#define NET_IF      "msx/me/if/NET0/"
#define NET_ADDR    "msx/me/if/NET0/conf/addr"
#define NET_PORT    "msx/me/if/NET0/conf/port"
#define NET_CONNECT "msx/me/if/NET0/connect"
#define NET_MSG     "msx/me/if/NET0/msg"

BOOL net_connect(const char *hostname, int port, int time_out);
// BOOL net_is_connected();
#define net_is_connected()      iot_geti(NET_CONNECT)
// void net_discconect();
#define net_discconect()        iot_puti(NET_CONNECT, 0)

// void net_write_str(const char *msg);
#define net_write_str(msg)          iot_puts(NET_MSG, msg)

#endif