#include<stdio.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<string.h>



int main(int argc, char**argv){
	int client_socket;
	client_socket=socket(AF_INET,SOCK_STREAM,0);
	if (client_socket==-1){perror("failed to create the client socket");return 1;}
	
	struct sockaddr_in server_address;
	server_address.sin_family=AF_INET;
	server_address.sin_port=htons(12000);
	server_address.sin_addr.s_addr=inet_addr("127.0.0.1");// different way of ip_address
	int connectReturn;
	if( (connectReturn=connect(client_socket,(struct sockaddr*) &server_address,sizeof(struct sockaddr)))==-1){
		perror("failed to connect to the server\n");
		return 1;
	}
	while(1){
		char client_message[1000]="0";
		printf("Enter the message: ");
		//scanf("%s",client_message);
		fgets(client_message,1000,stdin);
		int size_client_message=(int)strlen(client_message);
		client_message[size_client_message-1]='\0';
		char* endNode="\r\n\r\n";
		strcat(client_message, endNode);
		int sendSize=send(client_socket,client_message,1000,0);
		
		printf("my send size is %d\n",sendSize);
		if(sendSize==-1) perror("failed to send message\n");
		int recvSize=recv(client_socket,client_message,1000,0);
		if (recvSize==-1) perror("failed to receive message\n");
		printf("the message I received is %s\n",client_message);
	}
	close(client_socket);
	return 0;
}