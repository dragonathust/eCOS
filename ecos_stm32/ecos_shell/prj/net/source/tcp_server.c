#include <shell_err.h>
#include <shell_thread.h>
#include <commands.h>

#include <stdlib.h>

#include <network.h>
#include <sys/select.h>
#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>

static int port;
static int tcpServerTest(int port);
void tcpServer(cyg_addrword_t data);

shell_cmd("tcpserver",
	 "tcp_server test",
	 "",
	 tcpserver);

CMD_DECL(tcpserver){

	int i;
	port = 3000;
	
	for(i=0;i<argc;i++){

		if( (strncmp(argv[i],"port", 1))== 0){
			if( atoi( argv[++i]))
				port = atoi(argv[i]);			
		}
	}
	SHELL_PRINT("server port is %d\n",port);

	if (shell_create_thread(NULL, 6, tcpServer,(cyg_addrword_t) NULL, "ServerTest",
					NULL, SHELL_STACK_DEFAULT, NULL) != SHELL_OK){
					
		SHELL_ERROR("Failed to create Server_thread in %s\n", __FILE__);
	}

	//return tcpServerTest(port);

	return 0;
}

void tcpServer(cyg_addrword_t data){

	//shell_thread_t	*nt;
	//nt = (shell_thread_t*) data;
	int ret;
	struct sockaddr_in localAddr,remoteAddr;
	int serverfd;
	int sockfd;
	int addrLen;
	char recvBuf[1024];
	fd_set readSet,writeSet,excpSet;
	struct timeval timer;
	FD_ZERO(&readSet);
	FD_ZERO(&writeSet);
	FD_ZERO(&excpSet);
	
	if( (serverfd = socket(AF_INET,SOCK_STREAM,0)) < 0){

		SHELL_PRINT("socket err !\n");
		return ;
	}
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(port);
	localAddr.sin_addr.s_addr = INADDR_ANY;
	localAddr.sin_len = sizeof(localAddr);
	
	if( (ret = bind(serverfd, (struct sockaddr*)&localAddr, sizeof(struct sockaddr))) < 0 ){

		SHELL_PRINT("bind err !\n");
		return ;
	}
	if( (ret = listen(serverfd, 5)) < 0){

		SHELL_PRINT("listen err !\n");
		return ;
	}
	SHELL_PRINT("listen on the port %d \n",3000);
	addrLen = sizeof(struct sockaddr_in);
	if( (sockfd = accept(serverfd, (struct sockaddr*)&remoteAddr, &addrLen)) < 0 ){

		SHELL_PRINT("accept err !\n");
		return ;
	}
	SHELL_PRINT("get a connect from %s \n",inet_ntoa(remoteAddr.sin_addr));
	close(serverfd);

	timer.tv_sec = 3;
	timer.tv_usec = 1000;
	memset(recvBuf,'S',sizeof(recvBuf));
	int i=0;
	//while(++i < 10){
	while(1){
		FD_SET(sockfd,&readSet);
		FD_SET(sockfd,&writeSet);
		FD_SET(sockfd,&excpSet);
		ret = select(sockfd+1,&readSet,&writeSet,&excpSet,&timer);
		if( ret<0 ){
			close(sockfd);
			SHELL_PRINT("select err !\n");
			return ;
		}
		else if(ret == 0){
			SHELL_PRINT("select timer out \n");
			if(++i > 1000){
				close(sockfd);
				return;
			}
			continue;//timeout
		}
		else {

			if( FD_ISSET(sockfd,&readSet)){
				if( (ret = recv(sockfd, recvBuf, sizeof(recvBuf), 0)) < 0){
					close(sockfd);
					SHELL_PRINT("recv err !\n");
					return ;
				}
				//SHELL_PRINT("recv bufLen is %d \n",ret);
			}
			if( FD_ISSET(sockfd,&writeSet)){
				if( (ret = send(sockfd,recvBuf,sizeof(recvBuf),0)) < 0){
					close(sockfd);
					SHELL_PRINT("send err !\n");
					return ;
				}
				//SHELL_PRINT("send bufLen is %d \n",ret);
			}
			if( FD_ISSET(sockfd,&excpSet)){
				close(sockfd);
				SHELL_PRINT("select excp !\n");
				return ;
			}
		}//else ?
	
	}//while()???

	while(0){
		
		if( (ret = recv(sockfd, recvBuf, sizeof(recvBuf), 0)) < 0){
			close(sockfd);
			SHELL_PRINT("recv err !\n");
			return ;
		}
		SHELL_PRINT("recv bufLen is %d \n",ret);
	
		if( (ret = send(sockfd,recvBuf,sizeof(recvBuf),0)) < 0){
			close(sockfd);
			SHELL_PRINT("send err !\n");
			return ;
		}
		SHELL_PRINT("send bufLen is %d \n",ret);
	
	}
	close(sockfd);
	return ;
	
	
}

static int tcpServerTest(int port){

	int ret;
	struct sockaddr_in localAddr,remoteAddr;
	int serverfd;
	int sockfd;
	int addrLen;
	char recvBuf[1024];
	fd_set readSet,writeSet,excpSet;
	struct timeval timer;
	FD_ZERO(&readSet);
	FD_ZERO(&writeSet);
	FD_ZERO(&excpSet);
	
	if( (serverfd = socket(AF_INET,SOCK_STREAM,0)) < 0){

		SHELL_PRINT("socket err !\n");
		return -1;
	}
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(port);
	localAddr.sin_addr.s_addr = INADDR_ANY;
	localAddr.sin_len = sizeof(localAddr);
	
	if( (ret = bind(serverfd, (struct sockaddr*)&localAddr, sizeof(struct sockaddr))) < 0 ){

		SHELL_PRINT("bind err !\n");
		return -1;
	}
	if( (ret = listen(serverfd, 5)) < 0){

		SHELL_PRINT("listen err !\n");
		return -1;
	}
	SHELL_PRINT("listen on the port %d \n",port);
	addrLen = sizeof(struct sockaddr_in);
	if( (sockfd = accept(serverfd, (struct sockaddr*)&remoteAddr, &addrLen)) < 0 ){

		SHELL_PRINT("accept err !\n");
		return -1;
	}
	SHELL_PRINT("get a connect from %s \n",inet_ntoa(remoteAddr.sin_addr));
	close(serverfd);

	timer.tv_sec = 3;
	timer.tv_usec = 1000;
	memset(recvBuf,'S',sizeof(recvBuf));
	int i=0;
	//while(++i < 10){
	while(1){
		FD_SET(sockfd,&readSet);
		FD_SET(sockfd,&writeSet);
		FD_SET(sockfd,&excpSet);
		ret = select(sockfd+1,&readSet,&writeSet,&excpSet,&timer);
		if( ret<0 ){
			SHELL_PRINT("select err !\n");
			return -1;
		}
		else if(ret == 0){
			SHELL_PRINT("select timer out \n");
			if(++i > 10)return 0;
			continue;//timeout
		}
		else {

			if( FD_ISSET(sockfd,&readSet)){
				if( (ret = recv(sockfd, recvBuf, sizeof(recvBuf), 0)) < 0){
					SHELL_PRINT("recv err !\n");
					return -1;
				}
				//SHELL_PRINT("recv bufLen is %d \n",ret);
			}
			if( FD_ISSET(sockfd,&writeSet)){
				if( (ret = send(sockfd,recvBuf,sizeof(recvBuf),0)) < 0){
					SHELL_PRINT("send err !\n");
					return -1;
				}
				//SHELL_PRINT("send bufLen is %d \n",ret);
			}
			if( FD_ISSET(sockfd,&excpSet)){
				SHELL_PRINT("select excp !\n");
				return -1;
			}
		}//else ?
	
	}//while()???
	
	return 0;
}


