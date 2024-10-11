    include "iot.inc"


    SECTION code_user
    PUBLIC iot_node_write, _iot_node_write, iot_str_write
    GLOBAL strlen

; 文字列(データ)をIOT_PORT1に書き込む
;   hl <- 文字列の先頭アドレス
;   b <- 文字数

iot_str_write:
    ld a,0xc0
    out (IOT_PORT1),a

;   loop:
;    総文字数が64以上なら
;       文字数として0x7fを出力して
;       63文字分を出力
;       総文字数から63を引く
;   総文字数が64未満なら
;       文字数として総文字数を送信
;       総文字数文出力する。
;   文字列を出力
;   送信する文字列が残ってるならjp loop  
;
    ld c,IOT_PORT1
LOOP:
    ld a,b
    cp 0x40
    jr c,NEXT1

    ld a,0x7f
    out (IOT_PORT1),a
    ld a,b
    ld b,0x3f
    otir
    sub a,0x3f
    ld b,a
    jr LOOP

NEXT1:
    out (IOT_PORT1),a
    otir

    xor a
    out (IOT_PORT1),a

    ret


; nodeをIOT_PORT1に書き込む
;   hl <- node文字列の先頭アドレス
;   b <- 文字数
;
;    https://github.com/hra1129/for_MSX0/tree/main/sample_program/002_device/2023_05_30_1st_update_version/basicn
;    こちらのコードを元にしています。
;
;   戻り値
;       a <- マイナス値ならエラー
;
; int8_t iot_node_write(const char *node)
; {
;     outp(IOT_PORT1, 0xe0);
;     outp(IOT_PORT1, 1);
;     outp(IOT_PORT1, 0x53);

;     int len = strlen(node);
;     outp(IOT_PORT1, 0xc0);
;     outp(IOT_PORT1, len);

;     for(int i = 0; i < len; i++) {
;         outp(IOT_PORT1, node[i]);
;     }
;     outp(IOT_PORT1, 0);
;     int r = inp(IOT_PORT1);
;
;     retrun r;
; }

iot_node_write:
    ld a,0xe0
    out (IOT_PORT1),a
    ld a,1
    out (IOT_PORT1),a
    ld a,0x53
    out (IOT_PORT1),a

    call iot_str_write
    
    in a,(IOT_PORT1)        ; マイナス値ならエラーみたい。

    ret

; nodeをIOT_PORT1に書き込む(C言語)
; int8_t iot_node_write(const char *node);
_iot_node_write:
    ld hl,2
    add hl,sp

    ld e,(hl)
    inc hl
    ld d,(hl)

    ld h,d
    ld l,e
    ; 文字数をカウントする。
    call strlen

    ; iot_node_writeを呼ぶ
    ex hl,de

    call iot_node_write

    ld l,a
    ret