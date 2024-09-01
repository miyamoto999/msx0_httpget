
; typedef struct _rbuf {
;     char *buf;
;     int read_idx;
;     int write_idx;
;     int buf_mask;
; } RBUF;
; 
RBUF_BUF_OFFSET:        equ     0
RBUF_READ_IDX_OFFSET:   equ     2
RBUF_WRITE_IDX_OFFSET:  equ     4
RBUF_BUF_MASK_OFFSET:   equ     6
