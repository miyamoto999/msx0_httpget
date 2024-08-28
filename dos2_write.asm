    include "msxdos.inc"


    SECTION code_user
    PUBLIC _dos2_write


; /* ファイルハンドルへの書き込み */
; uint8_t dos2_write(uint8_t handle, void *buf, uint16_t count, uint16_t *bytes);

; sp + 2    bytes
; sp + 4    count
; sp + 6    buf
; sp + 8    handle
_dos2_write:
    ld hl,4
    add hl,sp

    ld e,(hl)
    inc hl
    ld d,(hl)
    inc hl

    ld c,(hl)
    inc hl
    ld b,(hl)
    inc hl
    
    ld a,(hl)

    ld h,b
    ld l,c
    
    ex hl,de

    ld b,a

    ld a,c

    ld c,_WRITE
    call BDOS

    ex hl,de

    ld hl,2
    add hl,sp

    ld c,(hl)
    inc hl
    ld b,(hl)

    ld l,c
    ld h,b

    ld (hl),e
    inc hl
    ld (hl),d

    ld l,a

    ret