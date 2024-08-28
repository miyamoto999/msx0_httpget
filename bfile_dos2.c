#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msxdos.h"
#include "buf_file.h"

struct __bfile_dos2 *bfile_create_dos2(const char *filename, int16_t buf_size)
{
    struct __bfile_dos2 *fp = malloc(sizeof(struct __bfile_dos2));
    if(!fp) {
        return NULL;
    }
    memset(fp, 0, sizeof(struct __bfile_dos2));

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

int16_t bfile_write_dos2(struct __bfile_dos2 *fp, const void *buf, int16_t size)
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

int bfile_close_dos2(struct __bfile_dos2 *fp)
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
