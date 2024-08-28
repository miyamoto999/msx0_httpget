    include "msxdos.inc"


    SECTION code_user
    PUBLIC _dos1_const


; /* コンソールステータス */
; uint8_t dos1_const();
_dos1_const:
    ld c,_CONST
    jp BDOS
