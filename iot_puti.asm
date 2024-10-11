    include "iot.inc"


    SECTION code_user
    PUBLIC iot_puti, _iot_puti
    GLOBAL iot_node_write,strlen

; nodeに数値を設定する。
;   hl <- nodeの文字列の先頭アドレス
;   b <- nodeの文字列の文字数
;   de <- 設定する値
; 戻り値
;   a <- 0:エラー、1:成功
;
;    https://github.com/hra1129/for_MSX0/tree/main/sample_program/002_device/2023_05_30_1st_update_version/basicn
;    こちらのコードを元にしています。
;
; void iot_puti(const char *node, const int val)
; {
;     iot_node_write(node);

;     outp(IOT_PORT1, 0xe0);
;     outp(IOT_PORT1, 1);
;     outp(IOT_PORT1, 0x41);

;     outp(IOT_PORT1, 0xc0);
;     outp(IOT_PORT1, 2);
;     outp(IOT_PORT1, val);
;     outp(IOT_PORT1, val >> 8);
; }


iot_puti:
    call iot_node_write
    rlca
    jr nc,NEXT

    xor a

    ret

NEXT:
    ld a, 0xe0
    out (IOT_PORT1),a
    ld a,1
    out (IOT_PORT1),a

    ld a,0x41               ; 整数の場合は0x41を与える
    out (IOT_PORT1),a

    ld a,0xc0
    out (IOT_PORT1),a
    ld a,2
    out (IOT_PORT1),a
    ld a,e
    out (IOT_PORT1),a
    ld a,d
    out (IOT_PORT1),a

    xor a
    out (IOT_PORT1),a

    inc a

    ret


; nodeに数値を設定する。(C言語)
; BOOL iot_puti(const char *node, const int val);
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
    call strlen
    pop hl

    call iot_puti

    ld l,a

    ret
