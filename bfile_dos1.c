#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "msxdos.h"
#include "buf_file.h"

struct __bfile_dos1 *bfile_create_dos1(const char *filename, int16_t buf_size)
{
    struct __bfile_dos1 *fp = malloc(sizeof(struct __bfile_dos1));
    if(!fp) {
        return NULL;
    }
    memset(fp, 0, sizeof(struct __bfile_dos1));
    msx_fcb_init(&fp->fcb, filename);
    fp->buf_size = buf_size > 0 ? buf_size:DEF_BUF_SIZE;
    fp->buf = malloc(fp->buf_size);
    if(!fp->buf) {
        free(fp);
        return NULL;
    }

    uint8_t err = dos1_fmake(&fp->fcb);
    if(err) {
        free(fp->buf);
        free(fp);
        return NULL;
    }
    fp->fcb.record_size = 1;
    fp->fcb.random_record = 0;

    return fp;
}

int16_t bfile_write_dos1(struct __bfile_dos1 *fp, const void *buf, int16_t size)
{
    uint8_t err;

    if(fp->err) {
        return 0;
    }

    if(size >= fp->buf_size) {
        if(fp->buf_offset != 0) {
            dos1_setdta(fp->buf);
            err = dos1_wrblk(&fp->fcb, fp->buf_offset);
            if(err) {
                fp->err = err;
                return 0;
            }
            fp->buf_offset = 0;
        }
        dos1_setdta(buf);
        err = dos1_wrblk(&fp->fcb, size);
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
                dos1_setdta(fp->buf);
                err = dos1_wrblk(&fp->fcb, fp->buf_offset);
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

int bfile_close_dos1(struct __bfile_dos1 *fp)
{
    uint8_t err = 0, err2;

    if(fp->buf_offset != 0 && fp->err == 0) {
        dos1_setdta(fp->buf);
        err = dos1_wrblk(&fp->fcb, fp->buf_offset);
    }

    err2 = dos1_fclose(&fp->fcb);

    if(err == 0 && err2) {
        err = err2;
    }

    free(fp->buf);
    free(fp);

    return (int)err;
}
