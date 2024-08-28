TARGET=test001.com
LD=z88dk-z80asm
AS=z88dk-z80asm
CC=zcc
OBJS=test001.o ../../msx_fcb_init.o
CFLAGS_INC=-I../../
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
	rm -f $(OBJS) $(TARGET) $(TARGET2) $(OBJS2) $(TARGET3) $(OBJS3) $(TARGET4) $(OBJS4) $(TARGET5) $(OBJS5) *.c.asm *.map *.img

depend:
	gcc $(CFLAGS_INC) -E -MM -w *.c > depends

-include depends
