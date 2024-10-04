TARGET=httpget.com
LD=z88dk-z80asm
AS=z88dk-z80asm
CC=zcc
OBJS=httpget.o dos1_const.o dos2_defab.o bfile_dos2.o dos2_create.o dos2_write.o dos2_close.o \
	iot_node_write.o net.o rbuf.o iot_puts.o iot_puti.o iot_geti.o rbuf_get_size.o rbuf_add_data.o iot_read.o \
	dos2_dosver.o rbuf_get_data.o rbuf_read.o dos1_dirio.o dos2_parse.o  dos2_pfile.o
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
dos1_dirio.o:dos1_dirio.asm msxdos.inc
dos2_defab.o:dos2_defab.asm msxdos.inc
dos2_create.o:dos2_create.asm msxdos.inc
dos2_write.o:dos2_write.asm msxdos.inc
dos2_close.o:dos2_close.asm msxdos.inc
dos2_dosver.o:dos2_dosver.asm msxdos.inc
dos2_parse.o:dos2_parse.asm msxdos.inc
dos2_pfile.o:dos2_pfile.asm msxdos.inc

iot_node_write.o:iot_node_write.asm iot.inc
iot_puts.o:iot_puts.asm iot.inc
iot_puti.o:iot_puti.asm iot.inc
iot_geti.o:iot_geti.asm iot.inc
iot_read_1.o:iot_read_1.asm iot.inc

rbuf_get_size.o:rbuf_get_size.asm rbuf.inc
rbuf_add_data.o:rbuf_add_data.asm rbuf.inc
rbuf_get_data.o:rbuf_get_data.asm rbuf.inc
rbuf_read.O:rbuf_read.asm rbuf.inc


-include depends
