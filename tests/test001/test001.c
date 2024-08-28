#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "buf_file.h"

void test1()
{
    struct msx_fcb fcb;
    BOOL ret;
    char *filename = "";

    printf("fcb init:\"%s\":", filename);
    ret = msx_fcb_init(&fcb, filename);
    assert(!ret);
    printf("OK\n");
}

void test2()
{
    struct msx_fcb fcb;
    BOOL ret;
    char *filename = "  ";

    printf("fcb init:\"%s\":", filename);
    ret = msx_fcb_init(&fcb, filename);
    assert(!ret);
    printf("OK\n");
}

void test3()
{
    struct msx_fcb fcb;
    BOOL ret;
    char *filename = "a:hoge";

    printf("fcb init:\"%s\":", filename);
    ret = msx_fcb_init(&fcb, filename);
    assert(ret);
    assert(fcb.drive == 1);
    assert(memcmp(fcb.name, "hoge       ", FCB_NAME_SIZE + FCB_EXT_SIZE) == 0);
    printf("OK\n");
}

void test4()
{
    struct msx_fcb fcb;
    BOOL ret;
    char *filename = "b:hoge.a";

    printf("fcb init:\"%s\":", filename);
    ret = msx_fcb_init(&fcb, filename);
    assert(ret);
    assert(fcb.drive == 2);
    assert(memcmp(fcb.name, "hoge    a  ", FCB_NAME_SIZE + FCB_EXT_SIZE) == 0);
    printf("OK\n");
}

void test5()
{
    struct msx_fcb fcb;
    BOOL ret;
    char *filename = "c:hogehogehoge.abcdef";

    printf("fcb init:\"%s\":", filename);
    ret = msx_fcb_init(&fcb, filename);
    assert(ret);
    assert(fcb.drive == 3);
    assert(memcmp(fcb.name, "hogehogeabc", FCB_NAME_SIZE + FCB_EXT_SIZE) == 0);
    printf("OK\n");
}

void test6()
{
    struct msx_fcb fcb;
    BOOL ret;
    char *filename = "hogehogehoge.abcdef";

    printf("fcb init:\"%s\":", filename);
    ret = msx_fcb_init(&fcb, filename);
    assert(ret);
    assert(fcb.drive == 0);
    assert(memcmp(fcb.name, "hogehogeabc", FCB_NAME_SIZE + FCB_EXT_SIZE) == 0);
    printf("OK\n");
}

void test7()
{
    struct msx_fcb fcb;
    BOOL ret;
    char *filename = "1:hoge.txt";

    printf("fcb init:\"%s\":", filename);
    ret = msx_fcb_init(&fcb, filename);
    assert(!ret);
    printf("OK\n");
}

void test8()
{
    struct msx_fcb fcb;
    BOOL ret;
    char *filename = "hogehogehogehoge";

    printf("fcb init:\"%s\":", filename);
    ret = msx_fcb_init(&fcb, filename);
    assert(ret);
    assert(fcb.drive == 0);
    assert(memcmp(fcb.name, "hogehoge   ", FCB_NAME_SIZE + FCB_EXT_SIZE) == 0);
    printf("OK\n");
}

void test9()
{
    struct msx_fcb fcb;
    BOOL ret;
    char *filename = ".hoge";

    printf("fcb init:\"%s\":", filename);
    ret = msx_fcb_init(&fcb, filename);
    assert(!ret);
    printf("OK\n");
}

void test10()
{
    struct msx_fcb fcb;
    BOOL ret;
    char *filename = "   a:hoge .txt";

    printf("fcb init:\"%s\":", filename);
    ret = msx_fcb_init(&fcb, filename);
    assert(ret);
    assert(fcb.drive == 1);
    assert(memcmp(fcb.name, "hoge    txt", FCB_NAME_SIZE + FCB_EXT_SIZE) == 0);
    printf("OK\n");
}

int main()
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    test8();
    test9();
    test10();

    return 0;
}

