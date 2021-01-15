# Makefile for CKcore target

.SUFFIXES: .o .c .cpp .cxx .s .S
.PHONY:	default all clean dirty

LDFLAGS = -nostartfiles -L$(ECOS_LIB_DIR) -Ttarget.ld

define PRINT_OBJS
	echo ""
	echo "# Objects linked to build/shell.elf:"
	echo "#########################################"
	cat build/obj.list
	echo ""
endef

define CHECK_RELEASE
	@for each_prj in $$(ls prj/* -d); do \
		if [ -e $$each_prj/Makefile ]; then \
			if [ -e $$each_prj/.release ]; then \
				$(MAKE) -C $$each_prj build; \
			else \
				echo ""; \
				echo "# $$each_prj not a Release."; \
				echo "*****************************************"; \
			fi; \
		else \
			echo ""; \
			echo "# $$each_prj no Makefile!."; \
			echo "*****************************************"; \
		fi; \
	done
endef

define CHECK_DIRTY
	@for each_prj in $$(ls prj/* -d); do \
		if [ -e $$each_prj/Makefile ]; then \
			$(MAKE) -C $$each_prj build; \
		else \
			echo ""; \
			echo "# $$each_prj no Makefile!."; \
			echo "*****************************************"; \
		fi; \
	done
endef

build: clean
	@mkdir -p build
	$(CHECK_RELEASE)
	$(MAKE) -C core build
	$(PRINT_OBJS)

dirty: clean
	@mkdir -p build
	$(CHECK_DIRTY)
	$(MAKE) -C core build
	$(PRINT_OBJS)

clean:
	@rm -rf build
	@for each_prj in $$(ls prj/* -d); do \
		if [ -e $$each_prj/Makefile ]; then \
			$(MAKE) -C $$each_prj $@; \
		fi; \
	done
	$(MAKE) -C core $@

cleanobj:
	@rm -f build/shell.elf

all: build

