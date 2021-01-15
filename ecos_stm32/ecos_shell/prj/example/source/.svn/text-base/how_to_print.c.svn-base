#include <shell_err.h>
#include <commands.h>

shell_cmd("hello_print", \
	  "Please use SHELL_PRINT() for general print.", \
	  NULL, \
	  hello_print);

CMD_DECL(hello_print)
{
	SHELL_PRINT("We are in hello_print, please use SHELL_PRINT() for general print.\n");
	return SHELL_OK;
}
