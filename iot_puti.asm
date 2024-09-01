    include "iot.inc"


    SECTION code_user
    PUBLIC iot_puti, _iot_puti
    GLOBAL iot_node_write

; nodeに数値を設定する。
;   hl <- nodeの文字列の先頭アドレス
;   b <- nodeの文字列の文字数
;   de <- 設定する値
;
;    https://github.com/hra1129/for_MSX0/tree/main/sample_program/002_device/2023_05_30_1st_update_version/basicn
;    こちらのコードを元にしています。

iot_puti:
    call iot_node_write

    ld a, 0xe0
    out (IOT_PORT1),a
    ld a,1
    out (IOT_PORT1),a
    ld a,0x41
    out (IOT_PORT1),a

    ld a,0xc0
    out (IOT_PORT1),a
    ld a,2
    out (IOT_PORT1),a
    ld a,e
    out (IOT_PORT1),a
    ld a,d
    out (IOT_PORT1),a

    ret


; nodeに数値を設定する。(C言語)
; void iot_puti(const char *node, const int val);
_iot_puti:

    ; valを取り出して
    ld hl,2
    add hl,sp

    ld e,(hl)
    inc hl
    ld d,(hl)
    inc hl

    ; nodeのアドレスを取り出して
    ld a,(hl)
    inc hl
    ld h,(hl)
    ld l,a

    push hl
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
    pop hl

    jp iot_puti
