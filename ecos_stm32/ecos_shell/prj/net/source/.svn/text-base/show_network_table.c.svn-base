#include <shell_err.h>
#include <shell_thread.h>
#include <commands.h>

#include <stdlib.h>

#include <network.h>
//#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <sys/select.h>

typedef int pr_fun(const char *fmt, ...);
extern void show_network_tables(pr_fun *pr);

shell_cmd("nettable",
	 "show network tables",
	 "",
	 nettable);

CMD_DECL(nettable){

	show_network_tables(printf);

	return 0;
}

