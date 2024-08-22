#ifndef __MSXDOS_H__
#define __MSXDOS_H__

#include <stdint.h>
#include "bool.h"

struct msx_fcb {
    uint8_t     drive;      /* ドライブ番号 */
    char        name[8];    /* ファイル名 */
    char        ext[3];     /* ファイルタイプ */
    uint8_t     cur_blk;    /* カレントブロック */
    char        dmy1;
    uint16_t    record_size;    /* レコードサイズ */
    long        file_size;      /* ファイルサイズ */
    uint8_t     date[2];        /* 日付 */
    uint8_t     time[2];        /* 時刻 */
    uint8_t     dev_id;         /* デバイスID */
    uint8_t     dir_loc;        /* ディレクトリロケーション */
    uint16_t    start_cluster;  /* 戦闘クラスタ */
    uint16_t    end_cluster;    /* 最終クラスタ */
    uint16_t    relative_pos;   /* 相対位置 */
    uint8_t     cur_record;     /* カレントレコード */
    long        random_record;  /* ランダムレコード */
};

/* ディスク転送アドレスのセット */
void dos1_setdta(void *dta);
/* ファイルのクローズ */
int8_t dos1_fclose(void *fcb);
/* ファイルの作成 */
int8_t dos1_fmake(void *fcb);
/* ランダムブロック書き込み */
int8_t dos1_wrblk(void *fcb, uint16_t rec_num);
/* 直接コンソールI/O */
char dos1_dirio(char code);
/* アボート終了ルーチンの定義 */
void dos2_defab(BOOL (*abort_routine)(uint8_t err1, uint8_t err2));
/* コンソールステータス */
uint8_t dos1_const();
/* ディスクエラー処理ルーチンの定義 */
void dos2_defer(uint8_t (*dsker_routine)(uint8_t err, uint8_t drv, uint8_t flag, uint16_t sec_no));


#define ERR_STOP        0x9f
#define ERR_CTRLC       0x9e

#endif