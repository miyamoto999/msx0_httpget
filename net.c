#include <time.h>
#include "bool.h"
#include "iot.h"
#include "net.h"

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
