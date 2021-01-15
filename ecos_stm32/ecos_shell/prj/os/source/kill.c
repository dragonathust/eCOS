#include <ctype.h>
#include <stdlib.h>

#include <shell_err.h>
#include <shell_thread.h>
#include <commands.h>

shell_cmd("kill",
	 "Kills a running thread",
	 "[thread ID]",
	 thread_kill);

CMD_DECL(thread_kill)
{
    cyg_handle_t th;
    char *erptr = NULL;

    if(argc != 1) {
	SHELL_DEBUG_PRINT("Usage: kill [tid]\n");
	return SHELL_OK;
    }

    th = strtol((char *) argv[0], &erptr, 0);

    if(*erptr) {
	SHELL_PRINT("Invalid thread handle\n");
	return SHELL_OK;
    }

    cyg_thread_kill(th);
    cyg_thread_delete(th);

    return SHELL_OK;
}

