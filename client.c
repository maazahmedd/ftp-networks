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
	
	char buffer[SIZE]; //hold the message that is being sent
	char echo_message[SIZE]; //this will hold the message you are going to receive

	while(1)
	{
		bzero(buffer,sizeof(buffer)); //clear buffer
		printf("Enter A Message: ");
		fgets(buffer,sizeof(buffer),stdin); //takes user input and puts in buffer
       	buffer[strcspn(buffer, "\n")] = 0;  //remove trailing newline char from buffer, fgets does not remove it
       	if(strcmp(buffer,"BYE!")==0 || (strcmp(buffer,"bye!")==0) || (strcmp(buffer,"bye")==0) || (strcmp(buffer,"BYE")==0)) //compare the input string in buffer with different variations of bye to see if user wants to end
        {
        	send(server_sd,buffer,strlen(buffer), 0); //send the bye message to the server because it has to print it
        	printf("Closing Connection To Server\n");
        	close(server_sd); //but then close the socket
        	printf("Connection Closed!\n\n");
            break;
        }
        else if (strcmp(buffer,"")==0) //if control+c pressed or just an enter key pressed then dont send the message but close socket which will cause the bytes variable in server to be 0
        {
        	printf("Closing Connection To Server\n");
        	close(server_sd); //close socket
        	printf("Connection Closed!\n\n");
            break;
        }
        if(send(server_sd,buffer,strlen(buffer),0)<0) //send the buffer to the server and if it fails, give error
        {
            perror("send");
            exit(-1);
        }

        bzero(echo_message,sizeof(echo_message)); //reinitialize it
	    recv(server_sd, echo_message, sizeof(echo_message), 0); //receive the echoed message from the server and store in echo_message
	    printf("Server Response: %s\n\n", echo_message); //print			
	}

	return 0;
}
