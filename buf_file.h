#ifndef __BUF_FILE_H__
#define __BUF_FILE_H__

#include <stdint.h>
#include "bool.h"
#include "msxdos.h"

#define DEF_BUF_SIZE    512

typedef struct __bfile_dos1 {
    struct msx_fcb fcb;
    char *buf;
    int16_t buf_size;
    uint16_t buf_offset;
    uint8_t err;
} BFILE_DOS1;

typedef struct __bfile_dos2 {
    uint8_t handle;
    char *buf;
    int16_t buf_size;
    uint16_t buf_offset;
    uint8_t err;
} BFILE_DOS2;

#ifdef __MSXDOS_MSXDOS1
typedef BFILE_DOS1 BFILE;
#else
typedef BFILE_DOS2 BFILE;
#endif

BOOL msx_fcb_init(struct msx_fcb *fcb, const char *filename);

struct __bfile_dos1 *bfile_create_dos1(const char *filename, int16_t buf_size);
int16_t bfile_write_dos1(struct __bfile_dos1 *fp, const void *buf, int16_t size);
int bfile_close_dos1(struct __bfile_dos1 *fp);

struct __bfile_dos2 *bfile_create_dos2(const char *filename, int16_t buf_size);
int16_t bfile_write_dos2(struct __bfile_dos2 *fp, const void *buf, int16_t size);
int bfile_close_dos2(struct __bfile_dos2 *fp);

#ifdef __MSXDOS_MSXDOS1
#define bfile_create(f, s)      bfile_create_dos1(f,s)
#define bfile_write(fp, b, s)   bfile_write_dos1(fp, b, s)
#define bfile_close(fp)         bfile_close_dos1(fp)
#elif defined(__MSXDOS_MSXDOS2)
#define bfile_create(f, s)      bfile_create_dos2(f,s)
#define bfile_write(fp, b, s)   bfile_write_dos2(fp, b, s)
#define bfile_close(fp)         bfile_close_dos2(fp)
#endif

#define bfile_error(fp)         (fp)->err
#define bfile_clearerr(fp)      (fp)->err = 0

#endif