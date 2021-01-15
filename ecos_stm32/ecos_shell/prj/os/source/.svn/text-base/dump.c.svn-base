#include <ctype.h>
#include <stdlib.h>

#include <shell_err.h>
#include <shell_thread.h>
#include <commands.h>

shell_cmd("dump",
	 "Shows a memory dump",
	 "",
	 hexdump);

CMD_DECL(hexdump)
{
    unsigned int i = 0, j = 0;
    unsigned int len = 100, width = 16;
    unsigned char *buf;
    char *cp = NULL;

    switch(argc) {

    case 1:
	buf = (unsigned char *)strtol((char *) argv[0], &cp, 0);

	if(cp && *cp) {
	    SHELL_PRINT("Value '%s' is not a valid address\n", (char *) argv[0]);
	    return SHELL_OK;
	}

	break;

    case 2:
	buf = (unsigned char *)strtol((char *) argv[0], &cp, 0);

	if(cp && *cp) {
	    SHELL_PRINT("Value '%s' is not a valid address\n", (char *) argv[0]);
	    return SHELL_OK;
	}   

	len = strtol((char *) argv[1], &cp, 0);

	if(cp && *cp) {
	    SHELL_PRINT("Value '%s' is not a valid length\n", (char *) argv[1]);
	    return SHELL_OK;
	}   	    

	break;

    case 3:
	buf = (unsigned char *)strtol((char *) argv[0], &cp, 0);

	if(cp && *cp) {
	    SHELL_PRINT("Value '%s' is not a valid address\n", (char *) argv[0]);
	    return SHELL_OK;
	}   

	len = strtol((char *) argv[1], &cp, 0);

	if(cp && *cp) {
	    SHELL_PRINT("Value '%s' is not a valid length\n", (char *) argv[1]);
	    return SHELL_OK;
	}   	    

	width = strtol((char *) argv[2], &cp, 0);

	if(cp && *cp) {
	    SHELL_PRINT("Value '%s' is not a valid width\n", (char *) argv[2]);
	    return SHELL_OK;
	}   	    	
	
	break;

    default:
	SHELL_PRINT("Usage: hexdump [address] [length] [width]\n");
	return SHELL_OK;
    }

    SHELL_PRINT("%08X: ", (unsigned int)buf);
    
    for(i = 0; i < len; i++) {
	if(i && !(i % width)) {
	    j = i - width;
	    SHELL_PRINT("\t\t");
	    for(; j < i; j++) SHELL_PRINT("%c", isprint(buf[j]) ? buf[j] : '.');
	    SHELL_PRINT("\n%08X: ", (unsigned int)buf + i);
	    j = 0;
	}
	SHELL_PRINT("%02X ", buf[i]);
	j++;
    }
    
    if(j) {
	for(i = width - j; i > 0; i--) SHELL_PRINT("   ");
	SHELL_PRINT("\t\t"); 
	for(i = len - j; i < len; i++) SHELL_PRINT("%c", isprint(buf[i]) ? buf[i] : '.');
    }
    SHELL_PRINT("\n");

    return SHELL_OK;
}

