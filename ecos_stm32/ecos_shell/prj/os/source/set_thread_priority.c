#include <ctype.h>
#include <stdlib.h>

#include <shell_err.h>
#include <shell_thread.h>
#include <commands.h>

shell_cmd("sp",
	 "Sets a threads priority",
	 "[thread ID]",
	 set_priority);

CMD_DECL(set_priority)
{
    cyg_handle_t thandle = 0;
    cyg_priority_t cur_pri, set_pri;
    //unsigned int tid, pri;
    char *erptr = NULL;

    if(argc == 2) {
	thandle = strtol((char *) argv[0], &erptr, 0);
	
	if(erptr && *erptr) {
	    SHELL_PRINT("Value '%s' is not a valid thread ID\n", (char *) argv[1]);
	    return SHELL_OK;
	}   	    

	set_pri = strtol((char *) argv[1], &erptr, 0);
	
	if(erptr && *erptr) {
	    SHELL_PRINT("Value '%s' is not a valid thread priority\n", (char *) argv[2]);
	    return SHELL_OK;
	}

	cur_pri = cyg_thread_get_current_priority(thandle);
	
	SHELL_PRINT("Changing thread %d priority from %d to %d\n", thandle, cur_pri, set_pri);

	cyg_thread_set_priority(thandle, set_pri);

	cur_pri = cyg_thread_get_current_priority(thandle);
	SHELL_PRINT("Thread %d priority now @ %d\n", thandle, cur_pri);
    }
    else SHELL_PRINT("Usage: sp [tid] [priority]\n");

    return SHELL_OK;
}

