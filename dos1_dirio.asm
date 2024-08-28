    include "msxdos.inc"


    SECTION code_user
    PUBLIC _dos1_dirio


; /* 直接コンソールI/O */
; char dos1_dirio(char code);
_dos1_dirio:
    ld hl,2
    add hl,sp

    ld e,(hl)

    ld c,_DIRIO
    jp BDOS
