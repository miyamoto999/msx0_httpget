    include "iot.inc"


    SECTION code_user
    PUBLIC iot_puts, _iot_puts
    GLOBAL iot_node_write, iot_str_write,strlen

; nodeに文字列を設定する。
;   hl <- nodeの文字列の先頭アドレス
;   b <- nodeの文字列の文字数
;   de <- 設定する文字列の先頭アドレス
;   c <- 設定する文字列の文字数
; 戻り値
;   a <- 0:失敗、1:成功
;
;    https://github.com/hra1129/for_MSX0/tree/main/sample_program/002_device/2023_05_30_1st_update_version/basicn
;    こちらのコードを元にしています。

iot_puts:
    push bc
    call iot_node_write
    pop bc

    rlca
    jr nc,NEXT

    xor a

    ret

NEXT:

    ld a, 0xe0
    out (IOT_PORT1),a
    ld a,1
    out (IOT_PORT1),a

    ld a,0x43           ; 値が文字列なら0x43
    out (IOT_PORT1),a

    ld b,c
    ex hl,de    
    call iot_str_write

    ld a,1

    ret


; nodeに文字列を設定する。(C言語)
; BOOL iot_puts(const char *node, const char *val);
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
    call strlen
    ld c,b

    ; nodeのアドレスを取り出して
    ld hl,4
    add hl,sp

    ld a,(hl)
    inc hl
    ld h,(hl)
    ld l,a

    push hl
    ; nodeの文字数をカウント
    call strlen
    pop hl

    call iot_puts

    ld l,a

    ret
