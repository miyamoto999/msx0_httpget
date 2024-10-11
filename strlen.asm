    SECTION code_user
    PUBLIC strlen

; 文字数をカウント
;   hl <- 文字列の先頭アドレス
;
; 戻り値
;   b <- 文字数
;
strlen:
    ld b,0
LOOP:
    ld a,(hl)
    or a
    ret z

    inc b
    inc hl
    jr LOOP
