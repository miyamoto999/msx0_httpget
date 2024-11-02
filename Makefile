TARGET=httpget.com
LD=z88dk-z80asm
AS=z88dk-z80asm
CC=zcc
OBJS=httpget.o 
CFLAGS_INC=-I./msxlib/include -D__MSXDOS__=2
CFLAGS=+msx -subtype=msxdos2
# CFLAGS=+msx -subtype=msxdos2 -compiler=sdcc
ASCONFIGS=$(CFLAGS_INC)

all:$(TARGET)

$(TARGET):$(OBJS) msxlib/libs/msxlib.lib msxlib/libs/doscall.lib msxlib/libs/iot.lib
	$(CC) $(CFLAGS) -create-app -m -o $(TARGET) $(OBJS) -L./msxlib/libs -lmsxlib -ldoscall -liot

.SUFFIXES: .asm .o
.asm.o:
	$(AS) $(ASCONFIGS) $<

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) $(CFLAGS_INC) -O --no-crt -c $<
#	$(CC) $(CFLAGS) $(CFLAGS_INC) -O --no-crt -S $<

clean:
	rm -f $(OBJS) $(TARGET) *.c.asm *.map *.img

depend:
	gcc $(CFLAGS_INC) -E -MM -w *.c > depends

-include depends
