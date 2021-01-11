
COMMAND_PREFIX = ckcore-elf-
INSTALL_DIR = $(shell pwd)/../

XCC           = $(COMMAND_PREFIX)gcc
XCXX          = $(XCC)
XLD           = $(COMMAND_PREFIX)ld
XAR           = $(COMMAND_PREFIX)ar

CFLAGS        =  -mbig-endian -g -O2 -Wall -Wstrict-prototypes  -DJAZZ -DCKCORE -DCHIP -DASSERT_ON -DPRINT_ON -DCACHE_ENABLE=1 -DCRT0_STK_SIZE=8192 -DDRV_SEL_AUDIO -DDRV_SEL_AVSYNC -DDRV_SEL_BUFFER -DDRV_SEL_GLOBAL -DDRV_SEL_GPI -DDRV_SEL_GPIO -DDRV_SEL_IRDA -DDRV_SEL_LEDKB -DDRV_SEL_OSD -DDRV_SEL_SUB -DDRV_SEL_VIDEO -DDRV_SEL_SPIC -DDRV_SEL_BPIC -DDRV_SEL_OTP -DDRV_SEL_PTI -DDRV_SEL_SMC -DDRV_SEL_TIMER -DDRV_SEL_UART -DDRV_SEL_WATCHDOG -DDRV_SEL_FLASH -D__ECOS

CXXFLAGS      = $(CFLAGS)
LDFLAGS_LIB       = -EB -r
ARFLAGS_LIB       = rcs

LDFLAGS       = --oformat elf32-ckcore-big -TLinkcmd.ld -EB -o
OS_LIBS = $(INSTALL_DIR)/lib/jazz/libucos.a
LIBS =  $(INSTALL_DIR)/lib/jazz/libjazz_taixin_lib.a $(INSTALL_DIR)/lib/jazz/libdrvbase.a  $(INSTALL_DIR)/lib/jazz/libm.a  $(INSTALL_DIR)/lib/jazz/libc.a $(INSTALL_DIR)/lib/jazz/libgcc.a

 
CFLAGS += -I$(INSTALL_DIR)/inc/
CFLAGS += -I$(INSTALL_DIR)/inc/os
CFLAGS += -I$(INSTALL_DIR)/inc/hal
CFLAGS += -I$(INSTALL_DIR)/inc/hal/arch
CFLAGS += -I$(INSTALL_DIR)/inc/util
CFLAGS += -I$(INSTALL_DIR)/inc/drv
CFLAGS += -I$(INSTALL_DIR)/inc/drv/drvbase
CFLAGS += -I$(INSTALL_DIR)/inc/drv/bus
CFLAGS += -I$(INSTALL_DIR)/inc/mdl
CFLAGS += -I$(INSTALL_DIR)/inc/ap
CFLAGS += -I$(INSTALL_DIR)/testframework
CFLAGS += -I$(INSTALL_DIR)/include
CFLAGS += -I$(INSTALL_DIR)/os/ucos_jazz/inc
CFLAGS += -I$(INSTALL_DIR)/jffs2/include

#OBJS = $(shell echo *.c)
OBJS_LIB = jffs2/build.o jffs2/compr.o jffs2/compr_rtime.o jffs2/compr_rubin.o \
 jffs2/debug.o jffs2/dir-ecos.o jffs2/erase.o  jffs2/gc.o \
 jffs2/nodelist.o jffs2/nodemgmt.o \
 jffs2/read.o jffs2/readinode.o jffs2/scan.o jffs2/write.o \
 jffs2/malloc-ecos.o jffs2/gcthread.o jffs2/flashio.o jffs2/fs-ecos.o \
 jffs2/crc32.o jffs2/rbtree.o jffs2/jffs2_porting.o jffs2_test.o
# jffs2/compr_zlib.o  

OBJS_LIB += fileio/file.o fileio/io.o fileio/fd.o fileio/misc.o fileio/dir.o

# RULES

.PHONY: all clean

all: libjffs2.a
app: test

clean:
	find -name "*.o"|xargs rm -f
	rm -f *.a *.elf *.map *.gz
	
%.o: %.c
	$(XCC) -c -o $*.o $(CFLAGS)  $<

%.o: %.cxx
	$(XCXX) -c -o $*.o $(CXXFLAGS)  $<

%.o: %.C
	$(XCXX) -c -o $*.o $(CXXFLAGS)  $<

%.o: %.cc
	$(XCXX) -c -o $*.o $(CXXFLAGS)  $<

libjffs2.a: $(OBJS_LIB)
	$(XAR) $(ARFLAGS_LIB)  $@ $(OBJS_LIB)

#	$(XLD) $(LDFLAGS_LIB)  -o $@ $(OBJS_LIB)

test:  $(OBJS) $(OBJS_LIB)
	$(XLD) $(LDFLAGS) $@.elf  $(OS_LIBS) $(OBJS) $(OBJS_LIB) $(LIBS) -Map $@.map
	ckcore-elf-objcopy -O binary $@.elf $@.bin
	rm -f *.gz
	gzip --best $@.bin
