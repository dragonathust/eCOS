#include <shell_err.h>
#include <commands.h>

shell_init_entry("hello_init", \
		"Show you how to add eCos_Shell extenal initilize func.", \
		hello_init);

INIT_DECL(hello_init)
{
	SHELL_PRINT("We are in hello_init, a extenal initilize func.\n");
	return;
}
