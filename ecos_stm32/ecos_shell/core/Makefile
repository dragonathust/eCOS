include ../config.mk

#PRJ_LIB_PATH = ../build/prj.a
SHELL_ELF = ../build/shell.elf

CFLAGS = -I./include -I$(ECOS_INCLUDE_DIR)
#LDFLAGS = -nostartfiles -L$(ECOS_LIB_DIR) -L$(PRJ_LIB_PATH) -Ttarget.ld
LDFLAGS = -nostartfiles -L$(ECOS_LIB_DIR) -Ttarget.ld

OBJECTS = source/builtin_commands.o \
	  source/init.o \
	  source/shell_err.o \
	  source/shell_thread.o \
	  source/shelltask.o \
	  source/thread_cleanup.o

$(OBJECTS):%.o:%.c
	$(CC) $(CFLAGS) $(ECOS_GLOBAL_CFLAGS) -c $< -o $@

build: $(OBJECTS)
	$(LD) $(LDFLAGS) $(ECOS_GLOBAL_LDFLAGS) $(OBJECTS) $$(cat ../build/obj.list) -o $(SHELL_ELF)

clean:
	rm -rf source/*.o
