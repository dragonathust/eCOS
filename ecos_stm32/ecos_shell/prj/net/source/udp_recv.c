#include <shell_err.h>
#include <shell_thread.h>
#include <commands.h>

#include <stdlib.h>

#include <network.h>

static int port;
void udpRecv(cyg_addrword_t data);

shell_cmd("udprecv",
	 "udp recv test",
	 "",
	 udprecv);

CMD_DECL(udprecv){

	int i;
	port = 3000;
	for(i=0;i<argc;i++){

		if( strncmp(argv[i],"port",1) == 0){
			if(atoi(argv[++i]))
				port = atoi(argv[i]);
		}
	}
	SHELL_PRINT("port is %d\n",port);
	
	if(shell_create_thread(NULL, 6, udpRecv,(cyg_addrword_t)NULL, "udpRecv",
		NULL, SHELL_STACK_DEFAULT,NULL) != SHELL_OK){

		SHELL_PRINT("failed to create thread in %s\n",__FILE__);
	}
	//return udpRecvTest(port);
	return 0;
}

void udpRecv(cyg_addrword_t data){

	int ret;
	int sockfd;
	struct sockaddr_in remoteAddr,localAddr;
	int addrLen;
	char recvBuf[1024];
	fd_set readSet;
	struct timeval timer;
	if( (sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0 ){
		SHELL_PRINT("sokcet err !\n");
		return ;
	}
	/*
	remoteAddr.sin_family = AF_INET;
	remoteAddr.sin_port = htonl(port);
	remoteAddr.sin_addr.s_addr = inet_addr(serverIp);
	remoteAddr.sin_len = sizeof(remoteAddr);
	*/
	addrLen = sizeof(remoteAddr);
	memset(recvBuf,0,sizeof(recvBuf));
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(port);
	localAddr.sin_len = sizeof(localAddr);
	localAddr.sin_addr.s_addr = INADDR_ANY;
	if( (ret = bind(sockfd, (struct sockaddr*)&localAddr, sizeof(struct sockaddr))) < 0){
		close(sockfd);
		SHELL_PRINT("bind err !\n");
		return ;
	}
	
	timer.tv_sec = 0;
	timer.tv_usec = 1000;
	FD_ZERO(&readSet);
	int i = 0;
	//while(++i < 10){
	while(1){
		FD_SET(sockfd,&readSet);
		ret = select(sockfd+1,&readSet,NULL,NULL,&timer);
		if(ret<0){
			close(sockfd);
			SHELL_PRINT("select err !\n");
			return;
		}
		else if(ret == 0){
			SHELL_PRINT("select timer out\n");
			if(++i > 1000){
				close(sockfd);
				return;
			}
			continue;
		}
		else{
			if(FD_ISSET(sockfd, &readSet))
				if((ret = recvfrom(sockfd, recvBuf, sizeof(recvBuf), 0, 
									(struct sockaddr*)&remoteAddr, &addrLen))<0){
					close(sockfd);
					SHELL_PRINT("recvfrom err !\n");
					return ;
				}
				//SHELL_PRINT("recv bufLen is %d \n",ret);
		}//else??
	}//while()???
	close(sockfd);
	return;
}


