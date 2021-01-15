#include <shell_err.h>
#include <shell_thread.h>
#include <commands.h>

#include <stdlib.h>
#include <stdio.h>

#include "miniupnpc.h"
#include "upnpcommands.h"
#include "upnperrors.h"

static void DisplayInfos(struct UPNPUrls * urls,
                         struct IGDdatas * data)
{
	char externalIPAddress[16];
	char connectionType[64];
	char status[64];
	char lastconnerr[64];
	unsigned int uptime;
	unsigned int brUp, brDown;
	long int timenow, timestarted;
	int r;
	UPNP_GetConnectionTypeInfo(urls->controlURL,
	                           data->first.servicetype,
							   connectionType);
	if(connectionType[0])
		printf("Connection Type : %s\n", connectionType);
	else
		printf("GetConnectionTypeInfo failed.\n");
	UPNP_GetStatusInfo(urls->controlURL, data->first.servicetype,
	                   status, &uptime, lastconnerr);
	printf("Status : %s, uptime=%us, LastConnectionError : %s\n",
	       status, uptime, lastconnerr);
	timenow = time(NULL);
	timestarted = timenow - uptime;
	printf("  Time started : %s", ctime(&timestarted));
	UPNP_GetLinkLayerMaxBitRates(urls->controlURL_CIF, data->CIF.servicetype,
			&brDown, &brUp);
	printf("MaxBitRateDown : %u bps   MaxBitRateUp %u bps\n", brDown, brUp);
	r = UPNP_GetExternalIPAddress(urls->controlURL,
	                          data->first.servicetype,
							  externalIPAddress);
	if(r != UPNPCOMMAND_SUCCESS)
		printf("GetExternalIPAddress() returned %d\n", r);
	if(externalIPAddress[0])
		printf("ExternalIPAddress = %s\n", externalIPAddress);
	else
		printf("GetExternalIPAddress failed.\n");
}

static void GetConnectionStatus(struct UPNPUrls * urls,
                               struct IGDdatas * data)
{
	unsigned int bytessent, bytesreceived, packetsreceived, packetssent;
	DisplayInfos(urls, data);
	bytessent = UPNP_GetTotalBytesSent(urls->controlURL_CIF, data->CIF.servicetype);
	bytesreceived = UPNP_GetTotalBytesReceived(urls->controlURL_CIF, data->CIF.servicetype);
	packetssent = UPNP_GetTotalPacketsSent(urls->controlURL_CIF, data->CIF.servicetype);
	packetsreceived = UPNP_GetTotalPacketsReceived(urls->controlURL_CIF, data->CIF.servicetype);
	printf("Bytes:   Sent: %8u\tRecv: %8u\n", bytessent, bytesreceived);
	printf("Packets: Sent: %8u\tRecv: %8u\n", packetssent, packetsreceived);
}

static void ListRedirections(struct UPNPUrls * urls,
                             struct IGDdatas * data)
{
	int r;
	int i = 0;
	char index[6];
	char intClient[16];
	char intPort[6];
	char extPort[6];
	char protocol[4];
	char desc[80];
	char enabled[6];
	char rHost[64];
	char duration[16];
	/*unsigned int num=0;
	UPNP_GetPortMappingNumberOfEntries(urls->controlURL, data->servicetype, &num);
	printf("PortMappingNumberOfEntries : %u\n", num);*/
	do {
		snprintf(index, 6, "%d", i);
		rHost[0] = '\0'; enabled[0] = '\0';
		duration[0] = '\0'; desc[0] = '\0';
		extPort[0] = '\0'; intPort[0] = '\0'; intClient[0] = '\0';
		r = UPNP_GetGenericPortMappingEntry(urls->controlURL,
		                               data->first.servicetype,
		                               index,
		                               extPort, intClient, intPort,
									   protocol, desc, enabled,
									   rHost, duration);
		if(r==0)
		/*
			printf("%02d - %s %s->%s:%s\tenabled=%s leaseDuration=%s\n"
			       "     desc='%s' rHost='%s'\n",
			       i, protocol, extPort, intClient, intPort,
				   enabled, duration,
				   desc, rHost);
				   */
			printf("%2d %s %5s->%s:%-5s '%s' '%s'\n",
			       i, protocol, extPort, intClient, intPort,
			       desc, rHost);
		else
			printf("GetGenericPortMappingEntry() returned %d (%s)\n",
			       r, strupnperror(r));
		i++;
	} while(r==0);
}

/* protofix() checks if protocol is "UDP" or "TCP"
 * returns NULL if not */
const char * protofix(const char * proto)
{
	static const char proto_tcp[4] = { 'T', 'C', 'P', 0};
	static const char proto_udp[4] = { 'U', 'D', 'P', 0};
	int i, b;
	for(i=0, b=1; i<4; i++)
		b = b && (   (proto[i] == proto_tcp[i])
		          || (proto[i] == (proto_tcp[i] | 32)) );
	if(b)
		return proto_tcp;
	for(i=0, b=1; i<4; i++)
		b = b && (   (proto[i] == proto_udp[i])
		          || (proto[i] == (proto_udp[i] | 32)) );
	if(b)
		return proto_udp;
	return 0;
}

/* Test function
 * 1 - get connection type
 * 2 - get extenal ip address
 * 3 - Add port mapping
 * 4 - get this port mapping from the IGD */
static void SetRedirectAndTest(struct UPNPUrls * urls,
                               struct IGDdatas * data,
							   const char * iaddr,
							   const char * iport,
							   const char * eport,
                               const char * proto)
{
	char externalIPAddress[16];
	char intClient[16];
	char intPort[6];
	int r;

	if(!iaddr || !iport || !eport || !proto)
	{
		fprintf(stderr, "Wrong arguments\n");
		return;
	}
	proto = protofix(proto);
	if(!proto)
	{
		fprintf(stderr, "invalid protocol\n");
		return;
	}

	UPNP_GetExternalIPAddress(urls->controlURL,
	                          data->first.servicetype,
							  externalIPAddress);
	if(externalIPAddress[0])
		printf("ExternalIPAddress = %s\n", externalIPAddress);
	else
		printf("GetExternalIPAddress failed.\n");

	r = UPNP_AddPortMapping(urls->controlURL, data->first.servicetype,
	                        eport, iport, iaddr, 0, proto, 0);
	if(r!=UPNPCOMMAND_SUCCESS)
		printf("AddPortMapping(%s, %s, %s) failed with code %d (%s)\n",
		       eport, iport, iaddr, r, strupnperror(r));

	r = UPNP_GetSpecificPortMappingEntry(urls->controlURL,
	                                 data->first.servicetype,
    	                             eport, proto,
									 intClient, intPort);
	if(r!=UPNPCOMMAND_SUCCESS)
		printf("GetSpecificPortMappingEntry() failed with code %d (%s)\n",
		       r, strupnperror(r));

	if(intClient[0]) {
		printf("InternalIP:Port = %s:%s\n", intClient, intPort);
		printf("external %s:%s %s is redirected to internal %s:%s\n",
		       externalIPAddress, eport, proto, intClient, intPort);
	}
}
static void RemoveRedirect(struct UPNPUrls * urls,  struct IGDdatas * data,const char * eport,	const char * proto)
{	int r;
	if(!proto || !eport){		
		fprintf(stderr, "invalid arguments\n");		
		return;	
	}	
	proto = protofix(proto);	
	if(!proto){		
		fprintf(stderr, "protocol invalid\n");		
		return;	
	}	
	r = UPNP_DeletePortMapping(urls->controlURL, data->first.servicetype, eport, proto, 0);	
	printf("UPNP_DeletePortMapping() returned : %d\n", r);
}

shell_cmd("upnp",
	 "upnp",
	 "",
	 upnp);
	 
CMD_DECL(upnp){

	char lanaddr[64];	/* my ip address on the LAN */
	struct UPNPDev * devlist = 0;
	const char * rootdescurl = 0;
	const char * multicastif = 0;
	const char * minissdpdpath = 0;
	struct UPNPDev * device;
	struct UPNPUrls urls;
	struct IGDdatas data;
	char **commandargv;
	int commandargc;
	char command;
	int i,ret;

	SHELL_PRINT("argc  is %d \n",argc);
	for(i=0;i<argc;i++){
		SHELL_PRINT("argv[%d]  is %s \n",i,argv[i]);
	}
	
/*	
	if(!argv[0] || (argv == 'a' &&argc <4) ||(argv == 'd'  &&argc <2) || (argv == 'r' &&argc < 2) ){

		fprintf(stderr, "Usage :\t%s [options] -a ip port external_port protocol\n\t\tAdd port redirection\n", argv[0]);
		fprintf(stderr, "       \t%s [options] -d external_port protocol [port2 protocol2]?[...]\n\t\tDelete port redirection\n", argv[0]);
		fprintf(stderr, "       \t%s [options] -s\n\t\tGet Connection status\n", argv[0]);
		fprintf(stderr, "       \t%s [options] -l\n\t\tList redirections\n", argv[0]);
		fprintf(stderr, "       \t%s [options] -r port1 protocol1 [port2 protocol2] [...]\n\t\tAdd all redirections to the current host\n", argv[0]);
		fprintf(stderr, "\nprotocol is UDP or TCP\n");		fprintf(stderr, "Options:\n");
		fprintf(stderr, "  -u url : bypass discovery process by providing the XML root description url.\n");
		fprintf(stderr, "  -m address : provide ip address of the interface to use for sending SSDP multicast packets.\n");
		fprintf(stderr, "  -p path : use this path for MiniSSDPd socket.\n");
		return 1;
	}
*/
	devlist = upnpDiscover(2000, multicastif, minissdpdpath, 0);
	if(devlist)
	{
		printf("List of UPNP devices found on the network :\n");
		for(device = devlist; device; device = device->pNext)
		{
			printf(" desc: %s\n st: %s\n\n",
				   device->descURL, device->st);
		}
		//UPNP_GetIGDFromUrl(rootdescurl, &urls, &data, lanaddr, sizeof(lanaddr));
		ret = UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr));
		switch(ret){

			case 1:
				SHELL_PRINT("Found valid IGD : %s\n",urls.controlURL);
				break;
			case 2:
				SHELL_PRINT("Found a (not connected?) IGD : %s\n",urls.controlURL);
				SHELL_PRINT("Try to connect anyway \n");
				break;
			case 3:
				SHELL_PRINT("UPnP device found. Is it a IGD? : %s\n",urls.controlURL);
				SHELL_PRINT("Try to connect anyway \n");
				break;
			default :
				SHELL_PRINT("Found device (igd?) : %s\n",urls.controlURL);
				SHELL_PRINT("Try to connect anyway \n");
				break;

		}
		
		if(strncmp(argv[0], "-l", 2)==0){

			DisplayInfos(&urls, &data);
			ListRedirections(&urls, &data);

		}
		else if(strncmp(argv[0],"-d",2)==0 ){

			for(i=1;i<argc;i+=2)
				RemoveRedirect(&urls,&data,argv[i],argv[i+1]);
		}
		else if(strncmp(argv[0], "-r", 2)==0){

			for(i=1;i<argc;i+=2)
				SetRedirectAndTest(&urls,&data,lanaddr,argv[i],argv[i],argv[i+1]);
		}
		else if(strncmp(argv[0],"-a",2)==0){
			SetRedirectAndTest(&urls,&data,argv[1],argv[2],argv[3],argv[4]);
		}
		else if(strncmp(argv[0],"-s",2)==0){
			GetConnectionStatus(&urls, &data);
		}
		else{

			SHELL_PRINT("-l  list redirections \n");
			SHELL_PRINT("-r  port1 protocol1 [port2 protocol2] [...] \n Add all redirections to the current host\n");
			SHELL_PRINT("-a ip port external_port protocol \n Add port reditection\n");
			SHELL_PRINT("-d external_port protocol  [external_port2 protocol2] [...] \n Del prot redirectons\n");
			SHELL_PRINT("-s get connection status \n");
		}
	}

		
		///SetRedirectAndTest(&urls, &data,lanaddr, "10001", "10001","udp");
	else{

		fprintf(stderr, "No IGD UPnP Device found on the network !\n");
	}
	
	return 0;
}

/*
int main(int argc,char **argv)
{
	char lanaddr[64];	// my ip address on the LAN //
	struct UPNPDev * devlist = 0;
	const char * rootdescurl = 0;
	const char * multicastif = 0;
	const char * minissdpdpath = 0;
	struct UPNPDev * device;
	struct UPNPUrls urls;
	struct IGDdatas data;

	printf("Hello Test \n");
	devlist = upnpDiscover(2000, multicastif, minissdpdpath, 0);
	if(devlist)
	{
		printf("List of UPNP devices found on the network :\n");
		for(device = devlist; device; device = device->pNext)
		{
			printf(" desc: %s\n st: %s\n\n",
				   device->descURL, device->st);
		}
	}

	//UPNP_GetIGDFromUrl(rootdescurl, &urls, &data, lanaddr, sizeof(lanaddr));
	UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr));

	DisplayInfos(&urls, &data);
	ListRedirections(&urls, &data);

	SetRedirectAndTest(&urls, &data,lanaddr, "10001",
//										   "10001","udp");
	DisplayInfos(&urls, &data);
	ListRedirections(&urls, &data);

	return 0;
}

*/
