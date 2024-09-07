/* MSX-DOS2用バッファードファイル生成関係 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msxdos.h"
#include "buf_file.h"

/*
  ファイルを生成する。

    filename:ファイル名
    buf_size:バッファサイズ

  戻り値
    BFILE_DOS2構造体のポインタ
    NULL:エラー
 */
BFILE_DOS2 *bfile_create_dos2(const char *filename, int16_t buf_size)
{
    BFILE_DOS2 *fp = malloc(sizeof(BFILE_DOS2));
    if(!fp) {
        return NULL;
    }
    memset(fp, 0, sizeof(BFILE_DOS2));

    fp->buf_size = buf_size > 0 ? buf_size:DEF_BUF_SIZE;
    fp->buf = malloc(fp->buf_size);
    if(!fp->buf) {
        free(fp);
        return NULL;
    }

    uint8_t err = dos2_create(filename, FILE_MODE_WRITE_ONLY, FILE_ATTR_NORMAL, &(fp->handle));
    if(err) {
        free(fp->buf);
        free(fp);
        return NULL;
    }
    return fp;
}

/*
  ファイル書き込み

    fp:BFILE_DOS2構造体のポインタ
    buf:書き込むデータへのポインタ
    size:データサイズ

  戻り値
      書き込んだサイズ
*/
int16_t bfile_write_dos2(BFILE_DOS2 *fp, const void *buf, int16_t size)
{
    uint16_t tmp_size;
    uint8_t err;

    if(fp->err) {
        return 0;
    }

    if(size >= fp->buf_size) {
        if(fp->buf_offset != 0) {
            err = dos2_write(fp->handle, fp->buf, fp->buf_offset, &tmp_size);
            if(err) {
                fp->err = err;
                return 0;
            }
            fp->buf_offset = 0;
        }
        err = dos2_write(fp->handle, buf, size, &tmp_size);
        if(err) {
            fp->err = err;
            return 0;
        }
    } else {
        int copy_size, rest_size = size;
        int idx = 0;
        while(rest_size != 0) {
            if(fp->buf_size - fp->buf_offset < rest_size) {
                copy_size = fp->buf_size - fp->buf_offset;
                rest_size -= copy_size;
            } else {
                copy_size = rest_size;
                rest_size = 0;
            }
            memcpy(&fp->buf[fp->buf_offset], &buf[idx], copy_size);
            fp->buf_offset += copy_size;
            idx += copy_size;
            if(fp->buf_offset == fp->buf_size) {
                err = dos2_write(fp->handle, fp->buf, fp->buf_offset, &tmp_size);
                if(err) {
                    fp->err = err;
                    return 0;
                }
                fp->buf_offset = 0;
            }
        }
    }
    return size;
}

/*
  ファイルのクローズ

      fp:BFILE_DOS2構造体のポインタ

  戻り値
    0:成功
    0以外:失敗
*/
int bfile_close_dos2(BFILE_DOS2 *fp)
{
    uint8_t err = 0, err2;
    uint16_t tmp_size;

    if(fp->buf_offset != 0 && fp->err == 0) {
        err = dos2_write(fp->handle, fp->buf, fp->buf_offset, &tmp_size);
    }

    err2 = dos2_close(fp->handle);

    if(err == 0 && err2) {
        err = err2;
    }

    free(fp->buf);
    free(fp);
    return err;
}
