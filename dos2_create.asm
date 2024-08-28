
    include "msxdos.inc"


    SECTION code_user
    PUBLIC _dos2_create


; /* ファイルハンドルの作成 */
; uint8_t dos2_create(const char *filepath, uint8_t mode, uint8_t attr, uint8_t *handle);
; sp + 2    handle
; sp + 4    attr
; sp + 6    mode
; sp + 8    filepath
_dos2_create:
    ld hl,4
    add hl,sp

    ld b,(hl)
    inc hl
    inc hl

    ld c,(hl)
    inc hl
    inc hl

    ld e,(hl)
    inc hl
    ld d,(hl)

    ld a,c

    ld c,_CREATE
    call BDOS


    ld hl,2
    add hl,sp

    ld e,(hl)
    inc hl
    ld d,(hl)

    ex hl,de

    ld (hl),b

    ld l,a

    ret