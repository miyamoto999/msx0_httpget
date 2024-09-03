    include "rbuf.inc"


    SECTION code_user
    PUBLIC rbuf_get_data, _rbuf_get_data


;
; リングバッファに入っているデータ取得する
;
; int rbuf_get_data(RBUF *rbuf)
; {
;     if(rbuf->read_idx == rbuf->write_idx) {
;         return -1;
;     }
;     int ret = rbuf->buf[rbuf->read_idx];
;     rbuf->read_idx = (rbuf->read_idx + 1) & rbuf->buf_mask;
;     return ret & 0xff;
; }
;
;   ix <- rbufのアドレス
;
; 戻り値
;   hl <- データ、-1:データなし
;
rbuf_get_data:
    ld l,(ix+RBUF_READ_IDX_OFFSET)
    ld h,(ix+RBUF_READ_IDX_OFFSET+1)

    ld a,(ix+RBUF_WRITE_IDX_OFFSET+1)
    xor h
    jr nz,GET_DATA
    ld a,(ix+RBUF_WRITE_IDX_OFFSET)
    xor l
    jr z,NO_DATA

GET_DATA:
    push hl
    ld e,(ix+RBUF_BUF_OFFSET)
    ld d,(ix+RBUF_BUF_OFFSET+1)

    add hl,de

    ld c,(hl)

    pop hl
    inc hl
    ld a,(ix+RBUF_BUF_MASK_OFFSET)
    and l
    ld (ix+RBUF_READ_IDX_OFFSET),a
    ld a,(ix+RBUF_BUF_MASK_OFFSET+1)
    and h
    ld (ix+RBUF_READ_IDX_OFFSET+1),a

    ld h,0
    ld l,c
    ret

NO_DATA:
    ld hl,-1
    ret

;
; リングバッファに入っているデータ取得する(C言語IF)
;
_rbuf_get_data:
    ld hl,2
    add hl,sp

    ld a,(hl)
    inc hl
    ld h,(hl)
    ld l,a

    push hl
    pop ix

    jp rbuf_get_data