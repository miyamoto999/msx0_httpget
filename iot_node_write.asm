    include "iot.inc"


    SECTION code_user
    PUBLIC iot_node_write, _iot_node_write


; nodeをIOT_PORT1に書き込む
;   hl <- node文字列の先頭アドレス
;   b <- 文字数
;
;    https://github.com/hra1129/for_MSX0/tree/main/sample_program/002_device/2023_05_30_1st_update_version/basicn
;    こちらのコードを元にしています。
iot_node_write:
    ld a,0xe0
    out (IOT_PORT1),a
    ld a,1
    out (IOT_PORT1),a
    ld a,0x53
    out (IOT_PORT1),a

    ld a,0xc0
    out (IOT_PORT1),a
    ld a,b
    out (IOT_PORT1),a

    ld c,IOT_PORT1
    otir

    xor a
    out (IOT_PORT1),a
    
    in a,(IOT_PORT1)

    ret

; nodeをIOT_PORT1に書き込む(C言語)
; void iot_node_write(const char *node);
_iot_node_write:
    ld hl,2
    add hl,sp

    ld e,(hl)
    inc hl
    ld d,(hl)

    ld h,d
    ld l,e
    ; 文字数をカウントする。
    ld b,0
loop1:
    ld a,(hl)
    or a
    jr z,end_countup
    inc b
    inc hl
    jr loop1
end_countup:
    ; iot_node_writeを呼ぶ
    ex hl,de
    jp iot_node_write