TARGET=httpget.com
LD=z88dk-z80asm
AS=z88dk-z80asm
CC=zcc
OBJS=httpget.o iotread.o dos1_const.o dos2_defab.o bfile_dos2.o dos2_create.o dos2_write.o dos2_close.o \
	iot_node_write.o net.o rbuf.o iot_puts.o iot_puti.o iot_geti.o rbuf_get_size.o rbuf_add_data.o iot_read_1.o
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
#	$(CC) $(CFLAGS) $(CFLAGS_INC) -O --no-crt -S $<

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
iot_puts.o:iot_puts.asm iot.inc
iot_puti.o:iot_puti.asm iot.inc
iot_geti.o:iot_geti.asm iot.inc
iot_read_1.o:iot_read_1.asm iot.inc

rbuf_get_size.o:rbuf_get_size.asm rbuf.inc
rbuf_add_data.o:rbuf_add_data.asm rbuf.inc


-include depends
