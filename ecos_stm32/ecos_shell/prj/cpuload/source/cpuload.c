#include <shell_err.h>
#include <commands.h>
#include <cyg/cpuload/cpuload.h>

cyg_handle_t ecos_shell_cpuload_handle;
cyg_cpuload_t ecos_shell_cpuload_st;

shell_init_entry("cpuload_init", \
		"Call cyg_cpuload_create, to Init cpuload process.", \
		cpuload_init);

INIT_DECL(cpuload_init)
{
	cyg_uint32 calibration;

	cyg_cpuload_calibrate(&calibration);

	cyg_cpuload_create(&ecos_shell_cpuload_st, calibration,&ecos_shell_cpuload_handle);

	return;
}

shell_cmd("top", \
	  "Print current cpu_load info.", \
	  "Caculate cpu load, during 100ms/1s/10s.", \
	  cpuload_top);

CMD_DECL(cpuload_top)
{
	cyg_uint32 average_point1s;
	cyg_uint32 average_1s;
	cyg_uint32 average_10s;


	cyg_cpuload_get(ecos_shell_cpuload_handle, &average_point1s, &average_1s, &average_10s);

	SHELL_PRINT("Current CPU load:\n");
	SHELL_PRINT("100ms:   %d\n", average_point1s);
	SHELL_PRINT("1s:      %d\n", average_1s);
	SHELL_PRINT("10s:     %d\n", average_10s);

	return SHELL_OK;
}
