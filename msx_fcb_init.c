#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "bool.h"
#include "buf_file.h"

/*
    FCBにファイル名を設定する。

        fcb:FCBのアドレス
        filename:ファイル名

    戻り値
        TRUE:成功
        FALSE:失敗
 */
BOOL msx_fcb_init(struct msx_fcb *fcb, const char *filename)
{
    memset(fcb, 0, sizeof(struct msx_fcb));
    memset(fcb->name, ' ', FCB_NAME_SIZE + FCB_EXT_SIZE);

    int idx_start = 0, idx_colon = -1;
    int len = strlen(filename);
    int idx_dot = len;
    char *p;
    int drv = 0;

    /* 最初のスペースはとばす */
    while(filename[idx_start] == ' ') {
        idx_start++;
    }
    /* : の位置を探す */
    p = strchr(&filename[idx_start], ':');
    if(p) {
        idx_colon = p - &filename[idx_start] + idx_start;
        if(idx_colon - idx_start  == 1) {
            drv = toupper(filename[idx_start]);
            if(!(drv >= 'A' && drv <= 'Z')) {
                return FALSE;
            }
            drv -= 'A';
            fcb->drive = drv + 1;
        }
        idx_start = idx_colon + 1;
    }
    /* 後ろから"."を探す */
    p = strrchr(&filename[idx_start], '.');
    if(p) {
        idx_dot = p - &filename[idx_start] + idx_start;
    }

    if(idx_start == idx_dot) {
        return FALSE;
    }
    if(idx_start == len) {
        return FALSE;
    }
    /* ファイル名をコピーする */
    for(int i = 0; i < FCB_NAME_SIZE; i++) {
        if(idx_start + i >= idx_dot) {
            break;
        }
        fcb->name[i] = filename[idx_start + i];
    }
    /* 拡張子部分をコピーする */
    for(int i = 0; i < FCB_EXT_SIZE; i++) {
        if(idx_dot + 1 + i >= len) {
            break;
        }
        fcb->ext[i] = filename[idx_dot + 1 + i];
    }
    return TRUE;
}
