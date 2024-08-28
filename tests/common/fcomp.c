#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fcomp.h"

#define BUF_SIZE    512

static char buf1[BUF_SIZE];
static char buf2[BUF_SIZE];

int fcomp(const char *filename1, const char *filename2)
{
    FILE *fp1, *fp2;
    int ret = 0;

    fp1 = fopen(filename1, "rb");
    if(!fp1) {
        fprintf(stderr, "%s open error\n", filename1);
        return -1;
    }
    fp2 = fopen(filename2, "rb");
    if(!fp2) {
        fprintf(stderr, "%s open error\n", filename2);
        fclose(fp1);
        return -1;
    }

    while(1) {
        size_t size1 = fread(buf1, 1, BUF_SIZE, fp1);
        size_t size2 = fread(buf2, 1, size1, fp2);

        if(size1 != size2) {
            ret = 1;
            break;
        }

        int cmp = memcmp(buf1, buf2, size1);
        if(cmp != 0) {
            ret = 1;
            break;
        }
        if(feof(fp1)) {
            break;
        }
        if(ferror(fp1) || ferror(fp2)) {
            ret = -1;
            break;
        }
    }
    
    fclose(fp1);
    fclose(fp2);

    return ret;
}