TARGET=test003.com
LD=z88dk-z80asm
AS=z88dk-z80asm
CC=zcc
OBJS=test003.o ../../bfile_dos1.o ../common/fcomp.o ../../msx_fcb_init.o ../../dos1_fmake.o ../../dos1_wrblk.o \
		../../dos1_fclose.o ../../dos1_setdta.o
CFLAGS_INC=-I../../ -I../common
CFLAGS=+msx -subtype=msxdos2
ASCONFIGS=$(CFLAGS_INC)

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -create-app -m -o $(TARGET) $(OBJS)

.SUFFIXES: .asm .o
.asm.o:
	$(AS) $(ASCONFIGS) $<

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) $(CFLAGS_INC) -O --no-crt -c $<

clean:
	rm -f $(OBJS) $(TARGET) *.c.asm *.map *.img

depend:
	gcc $(CFLAGS_INC) -E -MM -w *.c > depends

-include depends
