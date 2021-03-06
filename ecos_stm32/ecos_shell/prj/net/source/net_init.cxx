#include <shell_err.h>
#include <shell_thread.h>
#include <commands.h>

#include <network.h>
#include <dhcp.h>
#include <stdlib.h>

#include "gxcore_ecos_module.hxx"

#include "net_cmd.h"


static int network_init_static(const char *if_name,struct network_info *net_info);
externC unsigned int network_cable_link(void);

struct network_info net_info ={
		"192.168.120.2", 	// Ip Address
		"255.255.255.0",	// netmask IP
		"192.168.120.255",	// Broadcase IP
		"192.168.120.110",	// Gateway IP
		"192.168.120.51",	// Serverip Address
};

cyg_uint8 state;
struct dhcp_lease lease;
const char *eth_name =  "eth1";
struct bootp bootp_data;	
static int network_init_dhcp(void);

externC
void network_init(cyg_uint16 mode,struct network_info *net_info){

	if(mode == NETWORK_DHCP)
	{
		SHELL_PRINT("NETWORK_DHCP\n");
		network_init_dhcp();
		SHELL_PRINT("gateware is %d\n",eth0_bootp_data.bp_giaddr.s_addr);
	}
	if(mode == NETWORK_STATIC)
	{	
		SHELL_PRINT("NETWORK_STATIC\n");
		network_init_static(eth_name,net_info);
	}
}

int network_init_static(const char *if_name,struct network_info *net_info)
{

	build_bootp_record(
						&bootp_data,
						if_name,
						net_info->ipaddress,
						net_info->netmask,
						net_info->broadcast,
						net_info->gateway,
						net_info->serverip);
	if(!init_net(if_name, &bootp_data))
	{
		SHELL_PRINT("init_net failed !\n");
		return -1;
	}
	SHELL_PRINT("init_net succed.\n");
	return 0;

}
int network_init_dhcp(void)
{
	if( network_cable_link() )
	{
		init_all_network_interfaces();
		return 0;
	}
	SHELL_PRINT("network_init_dhcp()->no cable linked!!!!\n");
	return -1;
}

int network_connect_mode(const char* if_name,unsigned short cmd,struct network_info *net_info,cyg_uint8 *user,cyg_uint8 *password )
{
	int iRet = -1;

	switch(cmd)
	{
		case NETWORK_DHCP:
			iRet	= network_init_dhcp();
			if(iRet < 0)
			{
				SHELL_PRINT("Init Network Fail,Func = network_init_dhcp()\n");
			}
			else
			{
				break;
			}
		case NETWORK_STATIC:
			iRet	= network_init_static(if_name, net_info);
			if(iRet < 0)
			{
				SHELL_PRINT("Init Network Fail,Func = network_init_static() \n");
			}
			break;
		default:
			SHELL_PRINT("Please Choose The Right Command To Initial The Network \n");
			break;
	}

	if(iRet < 0)
	{
		
	}

	return iRet;
}


