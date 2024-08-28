    include "msxdos.inc"


    SECTION code_user
    PUBLIC _dos1_fclose


; /* ファイルのクローズ */
; int8_t dos1_fclose(void *dta);
_dos1_fclose:
    ld hl,2
    add hl,sp

    ld e,(hl)
    inc hl
    ld d,(hl)

    ld c,_FCLOSE
    jp BDOS
