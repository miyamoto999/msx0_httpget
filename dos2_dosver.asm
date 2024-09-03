
    include "msxdos.inc"


    SECTION code_user
    PUBLIC _dos2_dosver


; /* MSX-DOSのバーション番号の取得 */
; uint8_t dos2_dosver(uint16_t *kernel_ver, uint16_t *dos_ver);
_dos2_dosver:
    ld c,_DOSVER
    call BDOS

    ld hl,2
    add hl,sp

    push de
    ld e,(hl)
    inc hl
    ld d,(hl)
    
    ex hl,de
    pop de
    
    ld (hl),e
    inc hl
    ld (hl),d

    ld hl,4
    add hl,sp

    ld e,(hl)
    inc hl
    ld d,(hl)

    ex hl,de

    ld (hl),c
    inc hl
    ld (hl),b

    ld l,a

    ret