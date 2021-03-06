COMMON = print.o util.o ethernet.o arp.o tcp.o http.o
LOCAL = test.o spi.o test_spi.o divsi3.o
# note: the order of objects in the .ldr mirrors the order in this list!

CPUDEFINES = -D__ADSPBF527__  -D__BLACKFIN__
INCLUDES = -I../common_bfin/ -I/opt/uClinux/bfin-elf/bfin-elf/include/
ASMFLAGS = -x assembler-with-cpp $(INCLUDES) $(CPUDEFINES) -Wall
ASMFLAGS += -mcpu=bf527 -include memory.h -include defBF527.h
CFLAGS = $(INCLUDES) $(CPUDEFINES) -mcpu=bf527 -Wall -include memory.h
#CFLAGS += -O3 -ffast-math -mfast-fp -funsafe-math-optimizations 

ASMFLAGS += -g
CFLAGS += -O2 #-O2 #; -03 does not work!

# see http://blackfin.uclinux.org/gf/project/toolchain/forum/?action=ForumBrowse&forum_id=44&_forum_action=ForumMessageBrowse&thread_id=3023
# the archive file must have the same organization (FLAT, FDPIC ..)
LDFLAGS = -T bftiny.x \
 -L /opt/uClinux/bfin-uclinux/bfin-uclinux/runtime/usr/lib/
#-L /opt/uClinux/bfin-elf/bfin-elf/lib/bf532-none/mid-shared-library/ -lbffastfp
#  -L/opt/uClinux/bfin-elf/bfin-elf/lib/ -lbffastfp

COMSRC = $(COMMON:%.o=../common/%.c)
COMDBG = $(COMMON:%o=%.do)
OBJS = $(ASMSRCS:%.asm=%.o) $(CSRCS:%.c=%.o)
DBGOBJS = $(DBGASMSRCS:%.asm=%.o) $(CSRCS:%.c=%.do)
INITOBJS = $(INITSRC:%.asm=%.o)

# you'll need to install the bfin-elf toolchain to make this work. 
# see the blackfin Koop (URS may change, use google!)
BFINROOT = /opt/uClinux/bfin-elf/bin
AS = $(BFINROOT)/bfin-elf-as
CC = $(BFINROOT)/bfin-elf-gcc
LD = $(BFINROOT)/bfin-elf-ld
LDR = $(BFINROOT)/bfin-elf-ldr
OBDMP = $(BFINROOT)/bfin-elf-objdump
BIN = $(BFINROOT)/bfin-elf-objcopy

#svn version script runs at the /end/ of the build process, but this isn't such a problem as 
#the version does not change frequently. 
all: $(LOCAL) $(COMMON) test.dxe
#	perl svn_version.pl

%.o: %.asm
	$(CC) $(ASMFLAGS) -c -o $@ $<
	
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
%.o: ../common_bfin/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
%.o: ../common_bfin/%.asm
	$(CC) $(ASMFLAGS) -c -o $@ $<
	
# this is for loading with the JTAG cable (icebear, from section5.ch)
test.dxe: $(LOCAL) $(COMMON)
	./debug.pl bftiny.x > bftiny_dbg.x
	$(LD) -T bftiny_test.x -o $@ $(LOCAL) $(COMMON)
	$(OBDMP) -d $@ > test_decompile.asm

# this written to flash. 
test.ldr:test.dxe
	rm -f *.ldr
	$(LDR) -v -T BF527 -c $@ test.dxe
	$(OBDMP) -d test.dxe > decompile.asm
#	perl register_check.pl
	
clean:
	rm -f *.o *.do *.ldr *.dxe

flash: test.ldr
	#sudo ../flasher/flash test.ldr
	python ../flasher/flash.py test.ldr

reset:
	sudo ../flasher/flash --reset

debug: # you need to run bfin-gdbproxy bfin to start remote debugging.
	/opt/uClinux/bfin-elf/bin/bfin-elf-gdb
#useful commands: p /x $r0
# load emg_dbg.dxe 
# file emg_dbg.dxe
