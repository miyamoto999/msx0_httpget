    include "iot.inc"


    SECTION code_user
    PUBLIC iot_geti, _iot_geti
    GLOBAL iot_node_write

; nodeから数値を取得する
;   hl <- nodeの文字列の先頭アドレス
;   b <- nodeの文字列の文字数
;
;   戻り値
;   hl <- nodeの値
;
;    https://github.com/hra1129/for_MSX0/tree/main/sample_program/002_device/2023_05_30_1st_update_version/basicn
;    こちらのコードを元にしています。

iot_geti:
    call iot_node_write

    ld a, 0xe0
    out (IOT_PORT1),a
    ld a,1
    out (IOT_PORT1),a
    ld a,1
    out (IOT_PORT1),a
    ld a,0x80
    out (IOT_PORT1),a

    in a,(IOT_PORT1)

    in a,(IOT_PORT1)
    ld l,a
    in a,(IOT_PORT1)
    ld h,a

    ret


; nodeから数値を取得する(C言語IF)
; short iot_geti(const char *node);
_iot_geti:

    ld hl,2
    add hl,sp

    ; nodeのアドレスを取り出して
    ld e,(hl)
    inc hl
    ld d,(hl)
    
    ld h,d
    ld l,e
    ; nodeの文字数をカウント
    ld b,0
loop2:
    ld a,(hl)
    or a
    jr z,end_countup2
    inc b
    inc hl
    jr loop2
end_countup2:
    ex hl,de

    jp iot_geti
