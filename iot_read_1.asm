    include "iot.inc"


    SECTION code_user
    PUBLIC iot_read_1, _iot_read_1
    GLOBAL iot_node_write,rbuf_add_data

; nodeからデータを取得
;   hl <- nodeの文字列の先頭アドレス
;   b <- nodeの文字列の文字数
;   de <- データを保存するバッファの先頭アドレス
;   iy <- データ書き込みバッファサイズ
;   ix <- RBUF、リングバッファ
;
; 戻り値
;   hl <- 書き込んだデータ数
;
;
; int iot_read_1(RBUF *rbuf, const char *node, char *buf, int size)
; {
;     int ret = 0;

;     iot_node_write(node);

;     outp(IOT_PORT1, 0xe0);
;     outp(IOT_PORT1, 1);
;     outp(IOT_PORT1, 3);
;     outp(IOT_PORT1, 0x80);

;     int len = inp(IOT_PORT1);       // 試してみたかんじ、64バイト以上にはならないようだ。
;     for(int i = 0; i < len; i++) {
;         if(i >= size) {
;             rbuf_add_data(rbuf, inp(IOT_PORT1));
;         } else {
;             buf[i] = inp(IOT_PORT1);
;             ret++;
;         }
;     }
;     return ret;
; }

iot_read_1:
    push bc
    call iot_node_write
    pop bc

    ld a,0xe0
    out (IOT_PORT1),a
    ld a,1
    out (IOT_PORT1),a
    ld a,3
    out (IOT_PORT1),a
    ld a,0x80
    out (IOT_PORT1),a

    ld c,0
    in a,(IOT_PORT1)
    or a
    jp z,READ_RET

    ld b,a

    ex hl,de
    push iy
    pop de
loop1:
    in a,(IOT_PORT1)
    ld (hl),a
    inc hl
    inc c
    dec de
    ld a,d
    or e
    jr z,BUF_FULL

    dec b
    jr nz,loop1
READ_RET:
    ld h,0
    ld l,c
    ret

loop2:
    in a,(IOT_PORT1)
    push de
    call rbuf_add_data
    pop de

BUF_FULL:
    dec b
    jr nz,loop2
    ld h,0
    ld l,c
    ret

; nodeからデータを取得(C言語IF)
; int iot_read_1(RBUF *rbuf, const char *node, char *buf, int size);
; sp+2 size
; sp+4 buf
; sp+6 node
; sp+8 rbuf
_iot_read_1:
    ld hl,2
    add hl,sp

    ; bufを取り出す
    ld c,(hl)
    inc hl
    ld b,(hl)
    inc hl
    push bc
    pop iy

    ; sizeを取り出す
    ld e,(hl)
    inc hl
    ld d,(hl)
    inc hl

    ; nodeをいったんスキップ
    inc hl
    inc hl
    ; rbufを取り出す
    ld a,(hl)
    inc hl
    ld h,(hl)
    ld l,a
    push hl
    pop ix

    ; nodeを取り出す
    ld hl,6
    add hl,sp
    ld a,(hl)
    inc hl
    ld h,(hl)
    ld l,a

    push hl
    ld b,0
loop3:
    ld a,(hl)
    inc b
    inc hl
    or a
    jr nz,loop3
    pop hl
    dec b

    jp iot_read_1
