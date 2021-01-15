#include <shell_err.h>
#include <commands.h>

shell_cmd("hello_cmd", \
	  "Show you how to add eCos_Shell command.", \
	  "Write the Usage.", \
	  hello_command);

CMD_DECL(hello_command)
{
	int i;

	/* argc and argv are provied */
	SHELL_PRINT("We are in hello_command, argc=%d:\n", argc);

	for ( i=0; i<argc ; i++)
	{
		SHELL_PRINT("%s\n", argv[i]);
	}

	/* CMD_DECL need return */
	return SHELL_OK;
}
