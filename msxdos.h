#ifndef __MSXDOS_H__
#define __MSXDOS_H__

#include <stdint.h>
#include "bool.h"

#define FCB_NAME_SIZE   8
#define FCB_EXT_SIZE    3

struct msx_fcb {
    uint8_t     drive;      /* ドライブ番号 */
    char        name[FCB_NAME_SIZE];    /* ファイル名 */
    char        ext[FCB_EXT_SIZE];     /* ファイルタイプ */
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
/* コンソールステータス */
uint8_t dos1_const();

#ifdef __MSXDOS_MSXDOS2
/* アボート終了ルーチンの定義 */
void dos2_defab(BOOL (*abort_routine)(uint8_t err1, uint8_t err2));
/* ディスクエラー処理ルーチンの定義 */
void dos2_defer(uint8_t (*dsker_routine)(uint8_t err, uint8_t drv, uint8_t flag, uint16_t sec_no));

/* ファイルハンドルの作成 */
uint8_t dos2_create(const char *filepath, uint8_t mode, uint8_t attr, uint8_t *handle);
/* ファイルオープンモード */
#define FILE_MODE_READ_ONLY      0x01
#define FILE_MODE_WRITE_ONLY     0x02
#define FILE_MODE_READ_WRITE     0x00
#define FILE_MODE_INHELITANCE    0x04
/* ファイルアトリビュート */
#define FILE_ATTR_READONLY      0x01
#define FILE_ATTR_HIDDEN        0x02
#define FILE_ATTR_SYSTEM        0x04
#define FILE_ATTR_VOL           0x08
#define FILE_ATTR_SUBDIR        0x10
#define FILE_ATTR_ARCH          0x20
#define FILE_ATTR_DEVICE        0x80
#define FILE_ATTR_NORMAL        0x00
#define FILE_ATTR_NEW           0x80

/* ファイルハンドルのクローズ */
uint8_t dos2_close(uint8_t handle);
/* ファイルハンドルへの書き込み */
uint8_t dos2_write(uint8_t handle, void *buf, uint16_t count, uint16_t *bytes);
#endif

/* MSX-DOSのバーション番号の取得 */
uint8_t dos2_dosver(uint16_t *kernel_ver, uint16_t *dos_ver);

#define ERR_STOP        0x9f
#define ERR_CTRLC       0x9e

#endif