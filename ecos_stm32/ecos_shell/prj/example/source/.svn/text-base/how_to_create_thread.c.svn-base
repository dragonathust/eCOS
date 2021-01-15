#include <shell_err.h>
#include <commands.h>

/* shell_thread.h is needed.*/
#include <shell_thread.h>

shell_cmd("hello_task", \
	  "Show you how to create thread in eCos_Shell.", \
	  "Write the Usage.", \
	  hello_task);

static char hello_thread_str[] = "I'm hello_thread_str arguement.\n";

void
hello_thread_task(cyg_addrword_t data)
{
	shell_thread_t	*nt;

	nt = (shell_thread_t*) data;

	SHELL_PRINT("We are in hello_thread_task.\n");
	SHELL_PRINT("::%s\n", (char*) nt->arg);
}

CMD_DECL(hello_task)
{
	if(shell_create_thread(NULL, /* return eCos_Shell thread control handle*/
			      6,	/* priority value */
			      hello_thread_task, /* thread function handle */
			      (cyg_addrword_t) hello_thread_str, /* thread func arguement address */
			      "hello_thread", /* thread name */
			      NULL, /* stack address: if NULL, it will be created. */
			      SHELL_STACK_DEFAULT, /* stack size you want. */
			      NULL) /* cleanup function */
			     != SHELL_OK) {
	    SHELL_ERROR("Failed to create hello_thread in %s\n", __FILE__);
	}

	return SHELL_OK;
}
