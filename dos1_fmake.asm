    include "msxdos.inc"


    SECTION code_user
    PUBLIC _dos1_fmake


; /* ファイルの作成 */
; int8_t dos1_fmake(void *fcb);
_dos1_fmake:
    ld hl,2
    add hl,sp

    ld e,(hl)
    inc hl
    ld d,(hl)

    ld c,_FMAKE
    jp BDOS
