    include "msxdos.inc"


    SECTION code_user
    PUBLIC _dos2_pfile


; /* ファイル名の解析 */
; void dos2_pfile(const char *filenmae, char *buf, char **last_str, uint8_t *analysis_flag);
_dos2_pfile:
    ld hl,6
    add hl,sp

    ld e,(hl)
    inc hl
    ld d,(hl)
    inc hl

    ld a,(hl)
    inc hl
    ld h,(hl)
    ld l,a

    ex hl,de

    ld c,_PFILE
    call BDOS

    ld hl,2
    add hl,sp
    
    push de
    ld e,(hl)
    inc hl
    ld d,(hl)
    inc hl

    ex hl,de
    ld (hl),b
    ex hl,de

    ld e,(hl)
    inc hl
    ld d,(hl)

    ex hl,de
    pop de
    
    ld (hl),e
    inc hl
    ld (hl),d    

    ret    
