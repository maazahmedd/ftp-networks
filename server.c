#include<stdio.h> //header containing functions such as printf
#include<string.h> //header containing functions such as memset
#include<sys/socket.h> //header containing socket functions
#include<arpa/inet.h> //header containing byte-ordering standardization functions of htons
#include <netinet/in.h> //header containing structure for internet addresses and macros such as INADDR_ANY

#include<unistd.h> //for POSIX operating system API functions such as close
#include<stdlib.h> //header containing functions such as malloc, free, atoi

int main()
{
	//socket
	int server_sd = socket(AF_INET,SOCK_STREAM,0);
	if(server_sd<0)
	{
		perror("socket:");
		exit(-1);
	}
	//setsock
	int value  = 1;
	setsockopt(server_sd,SOL_SOCKET,SO_REUSEADDR,&value,sizeof(value)); //&(int){1},sizeof(int) //allows to reuse adress, without it one might run into "Address already in use" errors
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET; //IPV4 family
	server_addr.sin_port = htons(6000); //convert port number to network-byte order
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY, INADDR_LOOP

	//bind
	if(bind(server_sd, (struct sockaddr*)&server_addr,sizeof(server_addr))<0) //binds the socket to all the available network addresses which also include loopback (127.0.0.1) address
	{
		perror("bind failed");
		exit(-1);
	}
	//listen
	if(listen(server_sd,5)<0) //put socket in passive listening state and if listen call fails then print error
	{
		perror("listen failed");
		close(server_sd);
		exit(-1);
	}
	
	struct sockaddr_in cliaddr; // defines client address
    bzero(&cliaddr,sizeof(cliaddr));
    unsigned int len = sizeof(cliaddr);

    printf("Server Started: \n");
	printf("Server Is Listening...\n");
	while(1)
	{
		//accept
		int client_sd = accept(server_sd,(struct sockaddr*)&cliaddr, &len); //accept connection
		printf("[%s:%d] Connected!\n\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port)); //acknowledges a connection has been made
		
	}


	//recv/send

	//close
	close(server_sd);
	return 0;
}
