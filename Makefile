TARGET=httpget.com
LD=z88dk-z80asm
AS=z88dk-z80asm
CC=zcc
OBJS=httpget.o iot.o iotread.o dos1_const.o dos2_defab.o
CFLAGS_INC=
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

dos1_const.o:dos1_const.asm msxdos.inc
dos2_defab.o:dos2_defab.asm msxdos.inc

-include depends
