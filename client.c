//two components for the client side

//1) ftp user interface

//2) ftp client makes request to ftp server

//use fork() command for resource sharing

//use select command for simple connection making and simple commands

//client uses a random port (N > 1024?!?)

//server listens on port 21 (?!?) which is used as the control channel/exchanging commands

//for each data transfer, client sends PORT N+1 command to server

//the server listens on this port (N+1)

//server connects from port 20 to the client port to establish data channel

//for every consequent data transfer, the client sends PORT command again and announces new port N+2 or N+3 etc

//ports can be reused after some time


#include <stdio.h> // header for input and output from console : printf, perror
#include<string.h> // strcmp
//#include<sys/socket.h> // for socket related functions
#include<arpa/inet.h> // htons
#include <netinet/in.h> // structures for addresses

#include<unistd.h> // contains fork() and unix standard functions
#include<stdlib.h> //header containing functions such as mallox, free, atoi


#include<unistd.h> // header for unix specic functions declarations : fork(), getpid(), getppid()
#include<stdlib.h> // header for general fcuntions declarations: exit()

#define PORT 6000
#define SIZE 100

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
	setsockopt(server_sd,SOL_SOCKET,SO_REUSEADDR,&value,sizeof(value)); //&(int){1},sizeof(int) ////allows to reuse adress, without it one might run into "Address already in use" errors
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET; //IPV4 family
	server_addr.sin_port = htons(PORT); ////convert port number to network-byte order (PORT 6000 in this case)
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //INADDR_ANY, INADDR_LOOP

	//connect to the server and if it fails give error
    if(connect(server_sd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
    {
        perror("connect");
        exit(-1);
    }
    else
    {
    	printf("Connected to Server\n\n");
    }

	return 0;
}
