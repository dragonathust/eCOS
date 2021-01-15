#include <shell_err.h>
#include <shell_thread.h>
#include <commands.h>

#include <stdlib.h>

#include <network.h>
//#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <sys/select.h>
/**/
#include <cyg/hal/hal_arch.h>
#include <cyg/kernel/kapi.h>
#include <cyg/infra/testcase.h>
#include <cyg/infra/diag.h>

#ifdef CYGPKG_HAL_ARM
#include <cyg/cpuload/cpuload.h>
#endif

static char stack[CYGNUM_HAL_STACK_SIZE_MINIMUM];  
static cyg_handle_t handle; 
static cyg_thread thread;

#ifdef CYGPKG_HAL_ARM
cyg_uint32 calibration;
cyg_cpuload_t cpuload;
cyg_handle_t cpuloadhandle;
#endif

static int port;
static char *serverIp;
static int isRightAddr(const char *ch);
static int tcpClietTest(int port,char * serverIp);
/*
extern int cyg_select_with_abort(int nfd, fd_set *in, 
	fd_set *out, fd_set *ex, struct timeval *tv);
*/
void tcpCliet(cyg_addrword_t data);
void do_test(cyg_addrword_t data) ;

shell_cmd("tcpclient",
	 "tcp_client test",
	 "",
	 tcpclient);

CMD_DECL(tcpclient){

	int i;
	//int cmdFlag = 0;
	//int port = 3000;
	//char *serverIp = "192.168.120.154";
  	
	serverIp = "192.168.120.146";
	port = 3000;

#ifdef CYGPKG_HAL_ARM	
	cyg_cpuload_calibrate(&calibration);
	  	  
	 cyg_cpuload_create(&cpuload,calibration,&cpuloadhandle);

	 SHELL_PRINT("calibration is %d\n",calibration);
#endif

	for(i=0;i<argc;i++){

		if(strncmp(argv[i], "port", 1)==0)
			if(atoi(argv[++i]))
				port = atoi(argv[i]);
		if(strncmp(argv[i],"ip",2) == 0)
			if(isRightAddr(argv[++i]))
				serverIp = argv[i];

	}
	SHELL_PRINT("try connect to %s port is %d\n",serverIp,port);

	  

	if (shell_create_thread(NULL, 16, tcpCliet,(cyg_addrword_t) NULL, "tcpCliet",
					NULL, SHELL_STACK_DEFAULT, NULL) != SHELL_OK){
					
		SHELL_ERROR("Failed to create tcpCliet in %s\n", __FILE__);
	}

#ifdef CYGPKG_HAL_ARM	
	cyg_thread_create(30,do_test,0,"cpuload",stack,sizeof(stack),&handle,&thread);
  	cyg_thread_resume(handle);
#endif

 	 //cyg_scheduler_start();
	
	//return tcpClietTest(port, serverIp);
	return 0;
}

void tcpCliet(cyg_addrword_t data){
	
	int ret;
	int sockfd;
	int i = 0;
	fd_set readSet,writeSet,excpSet;
	struct timeval timer;
	struct sockaddr_in destAddr,localAddr;
	char recvBuf[2048];
	char sendBuf[1024];
	if( (sockfd = socket(AF_INET,SOCK_STREAM,0))< 0){
		SHELL_PRINT("socket err !\n");
		return;
	}

	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(3001);
	localAddr.sin_addr.s_addr = INADDR_ANY;
	localAddr.sin_len = sizeof(localAddr);
	
	if( (ret = bind(sockfd, (struct sockaddr*)&localAddr, sizeof(struct sockaddr))) < 0 ){

		SHELL_PRINT("bind err !\n");
		return ;
	}
	
	destAddr.sin_family = AF_INET;
	destAddr.sin_len = sizeof(destAddr);
	destAddr.sin_port = htons(port);
	destAddr.sin_addr.s_addr = inet_addr(serverIp);
	if((ret = connect(sockfd,(struct sockaddr*)&destAddr,sizeof(struct sockaddr))) < 0){
		SHELL_PRINT("connect errno is %d \n",errno);
		close(sockfd);
		SHELL_PRINT("connect err !\n");
		return ;
	}
	memset(recvBuf,'C',sizeof(recvBuf));
	memset(sendBuf,'S',sizeof(sendBuf));
	timer.tv_sec = 3;
	timer.tv_usec = 1000;
	FD_ZERO(&readSet);
	FD_ZERO(&writeSet);
	FD_ZERO(&excpSet);

	while(1){
/*	
		//FD_ZERO(&readSet);
		//FD_ZERO(&writeSet);
		//FD_ZERO(&excpSet);
		FD_SET(sockfd,&readSet);
		FD_SET(sockfd,&writeSet);
		FD_SET(sockfd,&excpSet);

		ret = select(sockfd+1, &readSet, &writeSet, &excpSet, &timer);
		//ret = select(sockfd+1, &readSet, NULL, &excpSet, &timer);
		if(ret < 0 )SHELL_PRINT("select err !\n");
		else if(ret==0){
			SHELL_PRINT("%d select timer out \n",i);//timer out
			if(++i > 100){
				close(sockfd);
				return ;
			}
			continue;
		}
		else{

			if( FD_ISSET(sockfd, &readSet)){

				if( (ret = recv(sockfd,recvBuf,sizeof(recvBuf),0)) < 0 ){
					close(sockfd);
					SHELL_PRINT("recv err ! \n");
					return ;
				}
				//SHELL_PRINT("recv bufLen is %d \n",ret);
			}
			if(FD_ISSET(sockfd,&writeSet)){

			
				if( (ret = send(sockfd,sendBuf,sizeof(sendBuf),0)) < 0 ){
					close(sockfd);
					SHELL_PRINT("send err ! \n");
					return ;
				}
				//SHELL_PRINT("send bufLen is %d \n",ret);
			
		
			}

			if(FD_ISSET(sockfd,&excpSet)){
				close(sockfd);
				SHELL_PRINT("select excp\n");
				return ;
			}
		}
 *//**/	if((ret = send(sockfd,sendBuf,sizeof(sendBuf),0))<0){

		close(sockfd);
		SHELL_PRINT("send err !");
		return;
	}
	//SHELL_PRINT("send bufLen is %d \n",ret);
	if( (ret = recv(sockfd,recvBuf,sizeof(recvBuf),0)) < 0 ){
		close(sockfd);
		SHELL_PRINT("recv err ! \n");
		//diag_printf("tcp_input  tcps_rcvtotal is %d \n", tcpstat.tcps_rcvtotal);
		//diag_printf("tcp_input  tcps_rcvbadsum is %d \n", tcpstat.tcps_rcvbadsum);
		return ;
	}
	//SHELL_PRINT("recv bufLen is %d \n",ret);
	}
	close(sockfd);
	return;
}

static int tcpClietTest(int port,char * serverIp){

	int ret;
	int sockfd;
	fd_set readSet,writeSet,excpSet;
	struct timeval timer;
	struct sockaddr_in destAddr;
	char recvBuf[1024*2];
	if( (sockfd = socket(AF_INET,SOCK_STREAM,0))< 0){
		SHELL_PRINT("socket err !\n");
		return -1;
	}
	destAddr.sin_family = AF_INET;
	destAddr.sin_len = sizeof(destAddr);
	destAddr.sin_port = htons(port);
	destAddr.sin_addr.s_addr = inet_addr(serverIp);
	if((ret = connect(sockfd,(struct sockaddr*)&destAddr,sizeof(struct sockaddr))) < 0){
		SHELL_PRINT("connect err !\n");
		return -1;
	}
	memset(recvBuf,'C',sizeof(recvBuf));
	timer.tv_sec = 1;
	timer.tv_usec = 0;
	FD_ZERO(&readSet);
	FD_ZERO(&writeSet);
	FD_ZERO(&excpSet);
	int i = 0;
	//while(++i < 10){
	while(1){
/*		if( (ret = recv(sockfd,recvBuf,sizeof(recvBuf),0)) < 0 ){
			SHELL_PRINT("recv err ! \n");
			return -1;
		}
		SHELL_PRINT("recv bufLen is %d \n",ret);
		
		if( (ret = send(sockfd,recvBuf,sizeof(recvBuf),0)) < 0 ){
			SHELL_PRINT("send errno is  %d ! \n",errno);
			return -1;
		}
		SHELL_PRINT("send bufLen is %d  errno is %d \n",ret,errno);
*/		
		
/**/	
		FD_ZERO(&readSet);
		FD_ZERO(&writeSet);
		FD_ZERO(&excpSet);
		FD_SET(sockfd,&readSet);
		FD_SET(sockfd,&writeSet);
		FD_SET(sockfd,&excpSet);

		ret = select(sockfd+1, &readSet, &writeSet, &excpSet, &timer);
		
		//ret = cyg_select_with_abort(sockfd+1, &readSet, &writeSet, &excpSet, &timer);
		if(ret < 0 )SHELL_PRINT("select err !\n");
		else if(ret==0){
			SHELL_PRINT("select timer out \n");//timer out
			if(++i > 100)return 0;
			continue;
		}
		else{

			if( FD_ISSET(sockfd, &readSet)){

				if( (ret = recv(sockfd,recvBuf,sizeof(recvBuf),0)) < 0 ){
					SHELL_PRINT("recv err ! \n");
					return -1;
				}
				SHELL_PRINT("recv bufLen is %d \n",ret);
			}
			if(FD_ISSET(sockfd,&writeSet)){

				if( (ret = send(sockfd,recvBuf,sizeof(recvBuf),0)) < 0 ){
					SHELL_PRINT("recv err ! \n");
					return -1;
				}
				SHELL_PRINT("send bufLen is %d \n",ret);
		
			}

			if(FD_ISSET(sockfd,&excpSet)){

				SHELL_PRINT("select excp\n");
				return -1;
			}
		}
		
	}

	return 0;	
}
static int isRightAddr(const char *ch){
	int a1,a2,a3,a4;	
	sscanf(ch,"%d.%d.%d.%d",&a1,&a2,&a3,&a4);
	if( a1>0 && a1<255 && a2>0 && a2<255 && a3>0 && a3<255 && a4>0 && a4<255)
		return 1;
	return 0;
}

#ifdef CYGPKG_HAL_ARM	
void
do_test(cyg_addrword_t data) {
//cyg_uint32 calibration;
//cyg_cpuload_t cpuload;
//cyg_handle_t cpuloadhandle;
/**/
  cyg_uint32 average_point1s;
  cyg_uint32 average_1s;
  cyg_uint32 average_10s;
  cyg_uint32 i,j;
  volatile unsigned long ttt;

/*   
  CYG_TEST_INFO("About to calibrate cpuload");
  
  cyg_cpuload_calibrate(&calibration);
  
  CYG_TEST_INFO("Performing 100% load test");
  
  cyg_cpuload_create(&cpuload,calibration,&cpuloadhandle);
*/ 
     
  while(1){
  	cyg_thread_delay(10);
  	cyg_cpuload_get(cpuloadhandle,&average_point1s,&average_1s,&average_10s);
  	diag_printf("**************%d***********%d********* %d\n",average_point1s,average_1s,average_10s);
  	//diag_printf("**************%d\n",average_point1s);
	//diag_printf("calibration is %d \n",calibration);
   }
  
  /*
  for (i=250; i--; ) {
    for (j=calibration*10; j-- ; ) 
      ;
    cyg_cpuload_get(handle,&average_point1s,&average_1s,&average_10s);
     diag_printf("%d %d %d\n",average_point1s,average_1s,average_10s);
  }


  if ((average_point1s == 100) &&
      (average_1s >= 99) && (average_1s <=100) &&
      (average_10s >= 98) && (average_10s <= 100))
    CYG_TEST_PASS("100% load o.k.");
  else {
    CYG_TEST_FAIL("100% load");
  }
  

  CYG_TEST_INFO("Performing 0% load test");
  for (i=350; i-- ; ) {
    cyg_thread_delay(10);
    cyg_cpuload_get(handle,&average_point1s,&average_1s,&average_10s);
     diag_printf("%d %d %d\n",average_point1s,average_1s,average_10s);
  }
  
  if ((average_point1s == 0) &&
      (average_1s == 0) &&
      (average_10s >= 0) && (average_10s <= 2))
    CYG_TEST_PASS_FINISH("0% load o.k.");
  else {
    CYG_TEST_FAIL_FINISH("0% load");
  }
  */
}
#endif

