/*
 * Copyright (c) 2005, 2006
 *
 * James Hook (james@wmpp.com) 
 * Chris Zimman (chris@wmpp.com)
 *
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cyg/hal/hal_diag.h>
#include <cyg/infra/cyg_type.h>		/* Atomic type */
#include <cyg/kernel/kapi.h>            /* All the kernel specific stuff */

#if defined(SHELL_TCPIP)
#include <network.h>			/* For the TCP/IP stack */
#endif	/* defined(SHELL_TCPIP) */

#define SHELL_DEFINES_GLOBALS
#include <shell.h>
#include <shell_err.h>
#include <shell_thread.h>
#include <commands.h>
#if defined(SHELL_DEBUG)
unsigned int shell_debug = 1;
#else
unsigned int shell_debug = 0;
#endif	/* defined(SHELL_DEBUG) */

CYG_HAL_TABLE_BEGIN(__shell_INIT_TAB__, shell_init_entries);
CYG_HAL_TABLE_END(__shell_INIT_TAB_END__, shell_init_entries);

/* Global thread count */
cyg_atomic shell_global_thnum = 0;

static void
shell_subsystem_init(void)
{
    /*
     * Put anything that needs to be initialized
     * outside of the driver scope but before
     * the system is "started" in here
     */

#if defined(SHELL_TCPIP)

    /* Initialize TCP/IP */
    SHELL_DEBUG_PRINT("Initializing TCP/IP\n");
    init_all_network_interfaces();

#endif	/* defined(SHELL_TCPIP) */
}

void
show_decl_funcs(void)
{
    initdesc_t *init_entry = __shell_INIT_TAB__;

    const char a[] = "Name", b[] = "Descriptions", c[] = "File Location";
    unsigned char d[sizeof(a) + sizeof(b) + sizeof(c) + 10 ];
    unsigned char i;

    if (init_entry == &__shell_INIT_TAB_END__) return;

    /*
     * Print extenal initilize function.
     */
    SHELL_PRINT("\n\nThe extenal initilize functions called:\n");
    snprintf((char*) d, sizeof(d) - 1, "%%-11s %%-60s %%-20s\n");
    SHELL_PRINT((char*)d, a, b, c);

    for(i = 0; i < sizeof(a) - 1; i++) putchar('-');
    SHELL_PRINT("        ");
    for(i = 0; i < sizeof(b) - 1; i++) putchar('-');
    SHELL_PRINT("                                                 ");
    for(i = 0; i < sizeof(c) - 1; i++) putchar('-');
    putchar('\n');

    while (init_entry != &__shell_INIT_TAB_END__)
    {
	SHELL_PRINT("%-11s %-60s %-20s\n",
		   init_entry->name,
		   init_entry->desc,
		   init_entry->file);
    	init_entry++;
    }
}

void
init_decl_start(void)
{
    initdesc_t *init_entry = __shell_INIT_TAB__;

    show_decl_funcs();

    /*
     * Excute extenal initilize function.
     */
    if (init_entry == &__shell_INIT_TAB_END__) return;

    while (init_entry != &__shell_INIT_TAB_END__)
    {
	init_entry->ifunc();
	init_entry++;
    }
}

void
cyg_user_start(void)
{
    /*
     * This is the entry point from the OS
     * Set everything up in here
     */

    SHELL_PRINT("BOOM! Creating the world...\n");

    /*
     * Call all INIT_DECL.
     */
    init_decl_start();

    /* Initialize I/O subsystems */
    shell_subsystem_init();

    /*
     * Create the 'init' equivalent thread
     * At least for the purposes of zombie prevention...
     */
    create_cleanup_thread();

#if !defined(SHELL_NO_SHELL)
    /*
     * Create the interactive shell thread
     */
    create_shell_thread();
#endif	/* !defined(SHELL_NO_SHELL) */

    main_cleanup();

    cyg_scheduler_start();
}

