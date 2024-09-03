    include "rbuf.inc"


    SECTION code_user
    PUBLIC rbuf_read, _rbuf_read
    GLOBAL rbuf_get_data


;
; リングバッファに入っているデータを取得する。
;
; int rbuf_read(RBUF *rbuf, char *buf, int size)
; {
;     int read_size = 0;

;     for(int i = 0; i < size; i++) {
;         int data = rbuf_get_data(rbuf);
;         if(data == -1) {
;             break;
;         }
;         buf[i] = data;
;         read_size++;
;     }
;     return read_size;
; }
;
;   ix <- rbufのアドレス
;   hl <- データを保存するバッファの先頭アドレス
;   de <- バッファのサイズ
;
; 戻り値
;   hl <- データ数
;
rbuf_read:
    ld bc,0
loop1:
    push hl
    push bc
    push de
    call rbuf_get_data
    pop de
    ld a,0xff
    xor h
    jr nz,NEXT
    ld a,0xff
    xor l
    jr z,DATA_END
NEXT:
    ld a,l
    pop bc
    pop hl
    ld (hl),a
    inc hl
    inc bc
    dec de

    ld a,d
    or e
    jr nz,loop1

    ld h,b
    ld l,c
    ret

DATA_END:
    pop hl      ; hl <- 元々のbc
    pop bc      ; 捨て
    ret

;
; リングバッファに入っているデータを取得する。(C言語IF)
;
; int rbuf_read(RBUF *rbuf, char *buf, int size);
;
_rbuf_read:
    ; sizeを取り出す
    ld hl,2
    add hl,sp
    ld e,(hl)
    inc hl
    ld d,(hl)
    inc hl

    ; bufを取り出す
    ld c,(hl)
    inc hl
    ld b,(hl)
    inc hl

    ; rbufを取り出す
    ld a,(hl)
    inc hl
    ld h,(hl)
    ld l,a
    push hl
    pop ix

    ld h,b
    ld l,c

    jp rbuf_read