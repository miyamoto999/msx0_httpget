    include "iot.inc"


    SECTION code_user
    PUBLIC iot_puts, _iot_puts
    GLOBAL iot_node_write

; nodeに文字列を設定する。
;   hl <- nodeの文字列の先頭アドレス
;   b <- nodeの文字列の文字数
;   de <- 設定する文字列の先頭アドレス
;   c <- 設定する文字列の文字数
;
;    https://github.com/hra1129/for_MSX0/tree/main/sample_program/002_device/2023_05_30_1st_update_version/basicn
;    こちらのコードを元にしています。

iot_puts:
    push bc
    call iot_node_write
    pop bc

    ld a, 0xe0
    out (IOT_PORT1),a
    ld a,1
    out (IOT_PORT1),a

    ld a,0x43           ; 値が文字列なら0x43
    out (IOT_PORT1),a

    ld b,c              ; nodeパスを送信する手順(途中から)と同じみたい？
    ld a,0xc0
    out (IOT_PORT1),a
    ld a,b
    out (IOT_PORT1),a

    ex hl,de
    
    ld c,IOT_PORT1
    otir

    xor a
    out (IOT_PORT1),a

    ; in a,(IOT_PORT1)  ; これいらない？

    ret


; nodeに文字列を設定する。(C言語)
; void iot_puts(const char *node, const char *val);
_iot_puts:

    ; valのアドレスを取り出して
    ld hl,2
    add hl,sp

    ld e,(hl)
    inc hl
    ld d,(hl)

    ld h,d
    ld l,e
    ; valの文字数をカウント
    ld c,0
loop1:
    ld a,(hl)
    or a
    jr z,end_countup1
    inc c
    inc hl
    jr loop1
end_countup1:

    ; nodeのアドレスを取り出して
    ld hl,4
    add hl,sp

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

    jp iot_puts
