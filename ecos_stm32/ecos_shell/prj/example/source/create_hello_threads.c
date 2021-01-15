#include <stdlib.h>

#include <shell_err.h>
#include <commands.h>
#include <shell_thread.h>

shell_cmd("hello_tasks", \
	  "create threads as many as you want.", \
	  "hello_tasks [num]", \
	  hello_tasks);

void
my_thread(cyg_addrword_t data)
{
	shell_thread_t	*nt;

	nt = (shell_thread_t*) data;
	while(1)
	{
		SHELL_PRINT("I'm %s.\n", (char*) nt->name);
		cyg_thread_delay(20);
	}
}

CMD_DECL(hello_tasks)
{
	int i, thread_num;
	char name[25];

	thread_num = atoi((char *)argv[0]);

	for (i=0; i<thread_num; i++)
	{
		sprintf(name, "hello_thread ID: %d", i);

		if (shell_create_thread(NULL, 6, my_thread,(cyg_addrword_t) NULL, name,
					NULL, SHELL_STACK_DEFAULT, NULL) != SHELL_OK)
		{
			SHELL_ERROR("Failed to create hello_thread in %s\n", __FILE__);
		}
	}

	return SHELL_OK;
}
