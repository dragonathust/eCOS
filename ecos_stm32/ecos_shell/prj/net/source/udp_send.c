#include <shell_err.h>
#include <shell_thread.h>
#include <commands.h>

#include <stdlib.h>

#include <network.h>
#include <arpa/inet.h>

static int bufLen;
static int port;
char* serverIp;
static int isRightAddr(const char *ch);
void udpSend(cyg_addrword_t data);

shell_cmd("udpsend",
	 "udp send test",
	 "",
	 udpsend);

CMD_DECL(udpsend){

	int i;	
	bufLen = 1024;
	serverIp = "192.168.120.154";
	port = 3000;
	for(i=0;i<argc;i++){

		if( strncmp(argv[i],"port",1) == 0){
			if(atoi(argv[++i]))
				port = atoi(argv[i]);
		}
		if(strncmp(argv[i],"ip",2)== 0){
			if(isRightAddr(argv[++i]))
				serverIp = argv[i];
		}
		if(strncmp(argv[i],"buflen",1)==0){
			if(atoi(argv[++i])){
				bufLen = atoi(argv[i]);
				if(bufLen <= 0)bufLen = 1;
				if(bufLen > 8192)bufLen = 8192;
			}
		}
	}
	SHELL_PRINT("server is %s and port is %d\n",serverIp,port);

	if(shell_create_thread(NULL,6, udpSend, (cyg_addrword_t)NULL,"udpSend",
			NULL, SHELL_STACK_DEFAULT,NULL) != SHELL_OK){
		SHELL_PRINT("failed to create udpSend in %s\n",__FILE__);
	}
	
	//return udpSendTest(port,serverIp,bufLen);
	return 0;
}

void udpSend(cyg_addrword_t data){

	int ret;
	int sockfd;
	struct sockaddr_in remoteAddr;
	char recvBuf[bufLen];
	fd_set writeSet;
	struct timeval timer;
	if( (sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0 ){
		SHELL_PRINT("sokcet err !\n");
		return;
	}
	
	remoteAddr.sin_family = AF_INET;
	remoteAddr.sin_port = htons(port);
	remoteAddr.sin_addr.s_addr = inet_addr(serverIp);
	remoteAddr.sin_len = sizeof(remoteAddr);
/*	
	addrLen = sizeof(remoteAddr);
	memset(recvBuf,0,sizeof(recvBuf));
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htonl(port);
	localAddr.sin_len = sizeof(localAddr);
	localAddr.sin_addr.s_addr = INADDR_ANY;
	if( (ret = bind(sockfd, (struct sockaddr*)&localAddr, sizeof(struct sockaddr))) < 0){
		SHELL_PRINT("bind err !\n");
		return -1;
	}
*/
	timer.tv_sec = 0;
	timer.tv_usec = 1000;
	FD_ZERO(&writeSet);
	int i = 0;
	//while(++i < 10){
	while(1){
		FD_SET(sockfd,&writeSet);
		ret = select(sockfd+1,NULL,&writeSet,NULL,&timer);
		if(ret<0){
			close(sockfd);
			SHELL_PRINT("select err !\n");
			return -1;
		}
		else if(ret == 0){
			SHELL_PRINT("select timer out \n");
			if(++i > 1000){
				close(sockfd);
				return 0;
			}
			continue;
		}
		else{
			if(FD_ISSET(sockfd, &writeSet))
				if((ret = sendto(sockfd, recvBuf, sizeof(recvBuf), 0, 
									(struct sockaddr*)&remoteAddr, sizeof(remoteAddr)))<0){
					close(sockfd);
					SHELL_PRINT("sendto err !\n");
					return;
				}
				//SHELL_PRINT("send bufLen is %d \n",ret);
		}//else??
	}//while()???
	close(sockfd);
	return 0;
}

static int isRightAddr(const char *ch){
	int a1,a2,a3,a4;	
	sscanf(ch,"%d.%d.%d.%d",&a1,&a2,&a3,&a4);
	if( a1>0 && a1<255 && a2>0 && a2<255 && a3>0 && a3<255 && a4>0 && a4<255)
		return 1;
	return 0;
}
