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

#include <cyg/infra/cyg_type.h>
#include <cyg/infra/diag.h>
#include <cyg/io/devtab.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/hal/plf_io.h>
#include <cyg/kernel/kapi.h>
#include <cyg/io/io.h>
#include <cyg/hal/hal_arch.h>

#include <ctype.h>
#include <stdlib.h>
#include <shell.h>
#include <shell_err.h>
#include <shell_thread.h>
#include <commands.h>

shell_cmd("help",
	 "Displays a list of commands",
	 "",
	 help_func);

void show_decl_funcs(void);

CMD_DECL(help_func)
{
    ncommand_t *shell_cmd = __shell_CMD_TAB__;

    const char cmds[] = "Commands", dsr[] = "Descriptions";
    const char usage[] = "Usage", location[] = "File Location";
    unsigned char helpar[sizeof(cmds) + sizeof(dsr) + sizeof(usage) + sizeof(location) + 10 ];
    unsigned char i;
    snprintf((char *)helpar, sizeof(helpar) - 1, "%%-11s %%-60s %%-20s %%-20s\n");
    
    SHELL_PRINT((char *)helpar, cmds, dsr, usage, location);

    for(i = 0; i < sizeof(cmds) - 1; i++) putchar('-');
    SHELL_PRINT("    ");
    for(i = 0; i < sizeof(dsr) - 1; i++) putchar('-');
    SHELL_PRINT("                                                 ");
    for(i = 0; i < sizeof(usage) - 1; i++) putchar('-');
    SHELL_PRINT("                ");
    for(i = 0; i < sizeof(location) - 1; i++) putchar('-');
    putchar('\n');

   while(shell_cmd != &__shell_CMD_TAB_END__) {
	SHELL_PRINT("%-11s %-60s %-20s %-20s\n",
		   shell_cmd->name,
		   shell_cmd->descr,
		   shell_cmd->usage,
		   shell_cmd->file);
	shell_cmd++;
   }

    show_decl_funcs();

    return SHELL_OK;
}


