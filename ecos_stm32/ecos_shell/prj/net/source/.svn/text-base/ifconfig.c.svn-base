#include <shell_err.h>
#include <shell_thread.h>
#include <commands.h>

#include <stdlib.h>

#include <network.h>
//#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>


static int isRightAddr(const char *ip);

shell_cmd("ifconfig",
	 "ifconfig",
	 "",
	 ifconfig);

CMD_DECL(ifconfig){
	int j;
	int i;
	int ret;
	int sockfd;
	struct ifreq ifr;
	struct sockaddr_in *addp;
	
	if(argc<1){
		//nettable;
	}
	else{
		addp = (struct sockaddr_in*)&ifr.ifr_addr;		
    	strcpy(ifr.ifr_name,"eth0");
		memset(addp,0,sizeof(*addp));
		addp->sin_family = AF_INET;
		addp->sin_len = sizeof(*addp);
		if((sockfd = socket(AF_INET,SOCK_DGRAM,0))<0){
			SHELL_PRINT("create socket err !\n");
			SHELL_PRINT("errno is %d \n",errno);
			return -1;
		}
		for(i=0;i<argc;i++){

			if(strncmp(argv[i],"ip",2)==0){

				if(!isRightAddr(argv[++i])){
					SHELL_PRINT("ip parameter err !\n");
					SHELL_PRINT("errno is %d \n",errno);
					close(sockfd);
					return -1;
				}
				SHELL_PRINT("ip is %s !\n",argv[i]);
				addp->sin_addr.s_addr = inet_addr(argv[i]);
				if((ret = ioctl(sockfd, SIOCSIFADDR, &ifr))<0){
					SHELL_PRINT("SIOCSIFADDR err ! \n");
					SHELL_PRINT("errno is %d \n",errno);
					close(sockfd);
					return -1;
				}
			}
			if(strncmp(argv[i],"netmask",3)==0){
				if(isRightAddr(argv[++i]))
					addp->sin_addr.s_addr = inet_addr(argv[i]);
				if((ret = ioctl(sockfd, SIOCSIFNETMASK, &ifr))<0){
					SHELL_PRINT("SIOCSIFNETMASK err !\n");
					SHELL_PRINT("errno is %d \n",errno);
					close(sockfd);
					return -1;
				}
			}
			if(strncmp(argv[i],"metric",4)==0){
				if(atoi(argv[++i]))
					ifr.ifr_metric = atoi(argv[i]);
				if((ret = ioctl(sockfd,SIOCSIFMETRIC,&ifr))<0){
					SHELL_PRINT("SIOCSIFMETRIC err !\n");
					SHELL_PRINT("errno is %d \n",errno);
					close(sockfd);
					return -1;
				}
			}
			if(strncmp(argv[i],"mtu",3)==0){
				if(atoi(argv[++i])>=0)
					ifr.ifr_metric = atoi(argv[i]);
				if((ret = ioctl(sockfd,SIOCSIFMTU,&ifr))<0){
					SHELL_PRINT("SIOCSIFMTU err !\n");
					SHELL_PRINT("errno is %d \n",errno);
					close(sockfd);
					return -1;
				}
			}			
			if(strncmp(argv[i],"up",2)==0){
				ifr.ifr_flags = IFF_UP;
				if((ret = ioctl(sockfd,SIOCSIFFLAGS,&ifr))<0){
					SHELL_PRINT("SIOCSIFFLAGS err !\n");
					SHELL_PRINT("errno is %d \n",errno);
					close(sockfd);
					return -1;
				}				
			}
			if(strncmp(argv[i],"down",4)==0){
				ifr.ifr_flags &= ~IFF_UP;
				if((ret = ioctl(sockfd,SIOCSIFFLAGS,&ifr))<0){
					SHELL_PRINT("SIOCSIFFLAGS err !\n");
					SHELL_PRINT("errno is %d \n",errno);
					close(sockfd);
					return -1;
				}				
			}
			if(strncmp(argv[i],"addbroadcast",4)==0){
				if(isRightAddr(argv[++i])){
					addp->sin_addr.s_addr = inet_addr(argv[i]);
					if((ret = ioctl(sockfd,SIOCADDMULTI, &ifr))<0){
						SHELL_PRINT("SIOCADDMULTI err !\n");
						SHELL_PRINT("errno is %d \n",errno);
						close(sockfd);
						return -1;
					}
				}
				else SHELL_PRINT("wrong broadcast addr !\n");
			}
			if(strncmp(argv[i],"delbroadcast",4)==0){
				if(isRightAddr(argv[++i])){
					addp->sin_addr.s_addr = inet_addr(argv[i]);
					if((ret = ioctl(sockfd,SIOCDELMULTI, &ifr))<0){
						SHELL_PRINT("SIOCDELMULTI err !\n");
						SHELL_PRINT("errno is %d \n",errno);
						close(sockfd);
						return -1;
					}
				}
				else SHELL_PRINT("wrong broadcast addr !\n");
			}
			if(strncmp(argv[i],"netstat",7)==0){
				struct ifstat arg;
				if((ret = ioctl(sockfd,SIOCGIFSTATUS,&arg))<0){
					SHELL_PRINT("SIOCGIFSTATUS err !\n");
					SHELL_PRINT("errno is %d !\n",errno);
					close(sockfd);
					return -1;
				}
				else{
					//memcpy(&arg, "test", 4);
					for(j=0;j<30;j++)
						SHELL_PRINT("the num %dth is  %d ",i,arg.ascii[j]);
					SHELL_PRINT("netstat: \n %s\n",arg.ascii);
				}
			}
		}

		close(sockfd);
	}
	
	return 0;
}

static int isRightAddr(const char *ip){
	int a1,a2,a3,a4;	
	sscanf(ip,"%d.%d.%d.%d",&a1,&a2,&a3,&a4);
	if( a1>0 && a1<255 && a2>0 && a2<255 && a3>0 && a3<255 && a4>0 && a4<255)
		return 1;
	return 0;
}

