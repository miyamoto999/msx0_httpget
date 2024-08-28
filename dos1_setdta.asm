    include "msxdos.inc"


    SECTION code_user
    PUBLIC _dos1_setdta


; /* ディスク転送アドレスのセット */
; void dos_setdta(void *dta);

_dos1_setdta:
    ld hl,2
    add hl,sp

    ld e,(hl)
    inc hl
    ld d,(hl)

    ld c, _SETDTA
    jp BDOS
