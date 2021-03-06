# Makefile for  program running in L1 memory
#this actually could be much simpler.
.SUFFIXES: .o .c .asm .bo .asm

ASMSRCS = crt0.o radio5.o enc.o divsi3.o
COMMON = print.o util.o spi.o
# CSRCS = main.o usb_write.o usb.o
CSRCS = main.o
# note: the order of objects in the .ldr mirrors the order in this list!


OPT = -O2
CPUDEFINES = -D__ADSPBF532__ -D__BLACKFIN__
INCLUDES = -I. -I/opt/uClinux/bfin-elf/bfin-elf/include/ -I../common_bfin/
ASMFLAGS = -x assembler-with-cpp $(INCLUDES) $(CPUDEFINES) -Wall
#ASMFLAGS += -mcpu=bf532 $(OPT) -include memory.h -include defBF532.h -D__ASM__
ASMFLAGS += -mcpu=bf532 -include memory.h -include defBF532.h -D__ASM__ -g
CFLAGS = $(INCLUDES) $(CPUDEFINES) -mcpu=bf532 $(OPT) -include memory.h -Wall
# -fomit-frame-pointer
# -ffixed-FP -ffixed-P5 -ffixed-P4 -ffixed-P0
# note: -O means debug symbols are not loaded.
#LDR = /home/tlh24/svn/bfin/ldr-utils/ldr
LDRFILE = $(shell ls *.ldr)

LDFLAGS = -T bftiny.x

OBJS = $(ASMSRCS) $(CSRCS) $(COMMON)
BOBJS = $(B_ASMSRCS:%.asm=%.bo) $(CSRCS:%.c=%.bo)

BFINROOT = /opt/uClinux/bfin-elf/bin
AS = $(BFINROOT)/bfin-elf-as
CC = $(BFINROOT)/bfin-elf-gcc
LD = $(BFINROOT)/bfin-elf-ld
LDR = $(BFINROOT)/bfin-elf-ldr
OBDMP = $(BFINROOT)/bfin-elf-objdump
BIN = $(BFINROOT)/bfin-elf-objcopy

%.bo: %.asm
	$(CC) $(ASMFLAGS) $(BRIDGE) -c -o $@ $<

%.o: ../common_bfin/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
%.o: ../common_bfin/%.asm
	$(CC) $(ASMFLAGS) -D_AGC_ -c -o $@ $<

%.o: %.asm
	$(CC) $(ASMFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


#note: delete spi.o each time, as it compiles different for the headstage.
# -- and we don't want to screw anything up.
#svn version script runs at the end of the build process, but this isn't such a problem as
#the version does not change frequently.
all: $(OBJS) stage.ldr stage.elf # radio_control.asm
	rm -f ../common/spi.o


stage.elf: $(OBJS)
	$(LD) $(LDFLAGS) -g -o stage.elf $(OBJS)

stage.dxe: $(OBJS)
	$(LD) $(LDFLAGS) -o stage.dxe $(OBJS)
	$(LD) $(LDFLAGS) -Map=Linker.map $(OBJS) 

enc_create: enc_create.cpp
	g++ -Wall $< -o $@

enc.asm: enc_create
	./enc_create

%.ldr:%.dxe
	rm -f *.ldr
	$(LDR) -T BF532 -c $@ $<
	$(OBDMP) -d $< > decompile.asm
	perl register_check.pl

record:
	sudo ttyUSB_client/usbtest
#	rm ttyUSB_client/out.txt
#	sudo ttyUSB_client/usbtest > ttyUSB_client/out.txt

clean:
	rm -f *.o *.ldr *.dxe radio_control.asm enc.asm enc_create Linker.map

flash:
	#sudo ../flasher/flash stage.ldr
	python ../flasher/flash.py stage.ldr
# If error claiming cannot find parallel port appears and you have a
# parallel port, execute:
# 	sudo rmmod lp
# 	sudo modprobe ppdev

reset:
	#sudo ../flasher/flash --reset
	python ../flasher/flash.py --reset

debug: # also need bfin-gdbproxy bfin --reset
	/opt/uClinux/bfin-elf/bin/bfin-elf-gdb

# To debug using gnICE through jtag, launch
#     bfin-gdbrpoxy -q bfin
# Then in a separate command prompt,
#	  bfin-elf-insight or bfin-elf-gdb stage.dxe
# For the latter, connect to "target remote :2000" 
