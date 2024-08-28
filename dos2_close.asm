    include "msxdos.inc"


    SECTION code_user
    PUBLIC _dos2_close


; /* ファイルハンドルのクローズ */
; uint8_t dos2_close(uint8_t handle);
_dos2_close:
    ld hl,2
    add hl,sp

    ld b,(hl)

    ld c,_CLOSE
    call BDOS

    ld l,a
    
    ret