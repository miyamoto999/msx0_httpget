    include "iot.inc"


    SECTION code_user
    PUBLIC iot_read,_iot_read
    GLOBAL iot_node_write,rbuf_add_data,rbuf_read, rbuf_get_size,strlen

; nodeからデータを取得
;   hl <- nodeの文字列の先頭アドレス
;   b <- nodeの文字列の文字数
;   de <- データを保存するバッファの先頭アドレス
;   iy <- データ書き込みバッファサイズ
;   ix <- RBUF、リングバッファ
;
; 戻り値
;   hl <- 読み込んだデータ数
;           -1:エラー
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

    rlca
    jr nc,NEXT

    ld hl,-1

    ret

NEXT:

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
    ld a,d
    or e
    jr z,loop2

    in a,(IOT_PORT1)
    ld (hl),a
    inc hl
    inc c
    dec de

    djnz loop1
READ_RET:
    ld h,0
    ld l,c
    ret

loop2:
    in a,(IOT_PORT1)
    call rbuf_add_data

    djnz loop2
    ld h,0
    ld l,c
    ret

; リングバッファにデータがある場合はリングバッファから、ない場合はnodeからデータを取得
;   hl <- nodeの文字列の先頭アドレス
;   b <- nodeの文字列の文字数
;   de <- データを保存するバッファの先頭アドレス
;   iy <- データ書き込みバッファサイズ
;   ix <- RBUF、リングバッファ
;
; 戻り値
;   hl <- 読み込んだデータ数
;           -1:エラー
;
; int iot_read(RBUF *rbuf, const char *node, char *buf, int size)
; {
;     int ret = 0;

;     if(rbuf_get_size(rbuf) == 0) {
;         ret = iot_read_1(rbuf, node, buf, size);
;     } else {
;         ret = rbuf_read(rbuf, buf, size);
;     }

;     return ret;
; }

iot_read:
    push hl
    push de
    call rbuf_get_size
    pop de
    ld a,h
    or l
    pop hl
    jp z,iot_read_1

    ex hl,de
    push iy
    pop de
    jp rbuf_read

; リングバッファにデータがある場合はリングバッファから、ない場合はnodeからデータを取得(C言語IF)
; int iot_read(RBUF *rbuf, const char *node, char *buf, int size);
_iot_read:
    ld hl,2
    add hl,sp

    ; sizeを取り出す
    ld c,(hl)
    inc hl
    ld b,(hl)
    inc hl
    push bc
    pop iy

    ; bufを取り出す
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
;     ld b,0
; loop4:
;     ld a,(hl)
;     inc b
;     inc hl
;     or a
;     jr nz,loop4
    call strlen
    
    pop hl
    ; dec b

    jp iot_read

