TARGET=httpget.com
LD=z88dk-z80asm
AS=z88dk-z80asm
CC=zcc
OBJS=httpget.o iot.o iotread.o dos1_const.o dos2_defab.o bfile_dos2.o dos2_create.o dos2_write.o dos2_close.o \
	iot_node_write.o net.o rbuf.o
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
dos2_create.o:dos2_create.asm msxdos.inc
dos2_write.o:dos2_write.asm msxdos.inc
dos2_close.o:dos2_close.asm msxdos.inc

iot_node_write.o:iot_node_write.asm iot.inc

-include depends
