# Git Test
OBJECTS := $(COMPILE:.cxx=.o)
OBJECTS := $(OBJECTS:.cpp=.o)
OBJECTS := $(OBJECTS:.c=.o)

%.o : %.c
	$(CC) $(CFLAGS) $(ECOS_GLOBAL_CFLAGS) -c $< -o $@

%.o : %.cxx
	$(CC) $(CFLAGS) $(subst -Wstrict-prototypes,,$(ECOS_GLOBAL_CFLAGS)) -c $< -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) $(subst -Wstrict-prototypes,,$(ECOS_GLOBAL_CFLAGS)) -c $< -o $@

print-info:
	echo ""
	echo "# prj$$(pwd | awk -F prj '{print $$2}') bulding:"
	echo "*****************************************"

build: print-info pre-build $(OBJECTS)
	for each_obj in $(OBJECTS); do \
		echo $$(pwd)/$$each_obj >> $(OBJ_LIST); \
	done

clean: pre-clean
	rm -rf source/*.o

