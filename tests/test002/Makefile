TARGET=test002.com
LD=z88dk-z80asm
AS=z88dk-z80asm
CC=zcc
OBJS=test002.o ../../bfile_dos2.o ../../dos2_create.o ../../dos2_write.o ../../dos2_close.o ../common/fcomp.o
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
