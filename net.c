#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bool.h"
#include "iot.h"
#include "net.h"
#include "msxdos.h"

#define BUF_SIZE    256
static char *retbuf;

/*************************************************
    メモ：ノード名から推測するに複数のネットワークや接続先も
        想定しいるように見えるけど、そういう拡張された時に
        対応すればいいかと思う。
 *************************************************/

/*
    ネットワーク接続(TCP接続)を使うための初期化

    戻り値
        TRUE:成功
        FALSE:失敗
 */
BOOL net_init()
{
    retbuf = malloc(BUF_SIZE);
    if(!retbuf) {
        return FALSE;
    }
    return TRUE;
}

/*
    ネットワーク接続(TCP接続)
    
        hostname:ホスト名またはIPアドレス
        port:ポート番号
        time_out:タイムアウト(sec)

    戻り値
        NET_ERR_NONE:成功
        その他：失敗
 */
int net_connect(const char *hostname, int port, int time_out)
{
    time_t st = time(NULL);

    iot_puts(NET_ADDR, hostname);
    iot_puti(NET_PORT, port);
    iot_puti(NET_CONNECT, 1);

    while(!net_is_connected()) {
        if(time(NULL) - st >= time_out) {
            return NET_ERR_TIMEOUT;
        }
        uint8_t key = dos1_dirio(0xff);
        if(key == 0x03 || key == 0xd3) {      /* DOS2のdos2_defab(アボート処理ルーチンの定義)で
                                                    CTRL-STOPで勝手に終了しないようにしている場合、
                                                    CTRL-STOPを押すと0xd3が返ってくるが、ドキュメントに
                                                    それっぽい記述が見つからない。けど、telnetでCtrl-Cを押すと
                                                    Ctrl-STOPを押したことになるようなので0xd3でもNET_ERR_ABORT
                                                    するようにした。
                                                    なお、DOS1ではCTRL-STOPを押すと0x03が返ってくる。
                                                    */
            return NET_ERR_ABORT;
        }
    }
    return NET_ERR_NONE;
}

/*
    接続先からデータを1バイト取得

        rbuf:リングバッファ

    戻り値
        -1以外:データ
        -1:データなし
 */
static int read_char(RBUF *rbuf)
{
    char data;
    int size;

    size = iot_read(rbuf, NET_MSG, &data, 1);
    if(size == 0) {
        return -1;
    }
    return data & 0xff;
}

/*
    ネットワーク(TCP接続)から1行取得

        rbuf:リングバッファ

    戻り値
        1行分の受信したデータ(LFまたはCRLFまでのデータ)
 */
char *net_readline(RBUF *rbuf)
{
    int size, idx;
    BOOL crflag = FALSE;

    idx = 0;

    while(1)
    {
        if(BUF_SIZE - 1 == idx) {
            retbuf[idx] = 0;
            break;
        }
        BOOL is_connected = iot_geti(NET_CONNECT);
        int ch = read_char(rbuf);
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
