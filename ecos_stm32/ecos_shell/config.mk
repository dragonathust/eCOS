
ARCH = arm
EXT = eabi

ARCH_EXT=$(ARCH)-$(EXT)-

ECOS_INSTALL_DIR = $(GXSRC_PATH)/build_stm32/install

ECOS_INCLUDE_DIR = $(ECOS_INSTALL_DIR)/include
ECOS_LIB_DIR = $(ECOS_INSTALL_DIR)/lib

include $(ECOS_INSTALL_DIR)/include/pkgconf/ecos.mak

CC = $(ARCH_EXT)gcc
LD = $(ARCH_EXT)gcc
AR = $(ARCH_EXT)ar

MAKE = make

OBJ_LIST = ../../build/obj.list

CFLAGS =  -O2 -I. -I./include -I./source -I../../core/include -I$(ECOS_INCLUDE_DIR)

