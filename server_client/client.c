#include<stdio.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "client.h"
#include "init_client.h"
      


int main(int argc, char**argv){
	char name[100],serverIP[100],serverPort[100];
	int cflags;
   if (init_client(argc,argv,name,serverIP,serverPort,&cflags)==0){
   		exit(EXIT_FAILURE);
   }
	long int converted_portNumber;
	if ( (converted_portNumber=convert_portNumber(serverPort))==-1){exit(EXIT_FAILURE);}
	printf("the port number is %ld\n",converted_portNumber);
	int client_socket;
	client_socket=socket(AF_INET,SOCK_STREAM,0);
	if (client_socket==-1){perror("failed to create the client socket");exit(EXIT_FAILURE);}
	struct sockaddr_in server_address;
	server_address.sin_family=AF_INET;
	server_address.sin_port=htons((uint16_t) converted_portNumber);
	//server_address.sin_port=htons(12000);
	//server_address.sin_addr.s_addr=inet_addr("127.0.0.1");// different way of ip_address
    if(inet_aton(serverIP, (struct in_addr *)&(server_address.sin_addr.s_addr))==0){
		perror("not a valid ip address\n");
		exit(EXIT_FAILURE);
	}	
	int connectReturn;
	if( (connectReturn=connect(client_socket,(struct sockaddr*) &server_address,sizeof(struct sockaddr)))==-1){
		perror("failed to connect to the server\n");
		return 1;
	}
	while(1){
		char client_message[1000]="0";
		printf("Enter the message: ");
		fgets(client_message,1000,stdin);
		int size_client_message=(int)strlen(client_message);
		client_message[size_client_message-1]='\0';
		char* endNode="\r\n\r\n";
		strcat(client_message, endNode);
		int sendSize=send(client_socket,client_message,strlen(client_message),0);
		if(sendSize==-1) perror("failed to send message\n");
		int recvSize=recv(client_socket,client_message,1000,0);
		if (recvSize==-1) perror("failed to receive message\n");
		printf("the message I received is %s\n",client_message);
		//client_login(client_socket,char*name,cflags);
	}
	close(client_socket);
	return 0;
}

int client_login(int client_socket,char*name, int cflags){
	//-1 disconnect -10 abnormal message from server 0 failed login 1 good 
	int sendSize, recvSize;
	char messageTosend[1000]="",messageReceive[1000]="",messageToCompare[1000]="";
	char * wolfie="WOLFIE";
	prepare_message(messageTosend,wolfie);
	if((sendSize=send(client_socket,messageTosend,strlen(messageTosend),0))==-1){
		perror("failed to send message\n");
		return -1;
	}
	if((recvSize=recv(client_socket,messageReceive,1000,0)) ==-1){
		perror("failed to receive message");
		return -1;
	} // 
	char* elflow="ELFLOW";
	prepare_message(messageToComapre,elflow);
	if(strcmp(messageReceive,messageToCompare)!=0){
		perror("not the message I expected\n");
		return -10;
	}	
	if (cflags==1){
	// create  new user
	}
	else if(cflags==0){
		return oldUserLogin(client_socket,name,messageTosend,messageToReceive, messageToCompare);
	}
	
	
	
	}
	
	return 1;
}

int oldUserLogin(int client_socket,char*name,char messageTosend[100], char messageToReceive[100],char messageToCompare[100]){
	char* Iam="IAM";
	strcpy(messageTosend,Iam);
	prepare_message(messageTosend,name);
		
	if((sendSize=send(client_socket,messageTosend,strlen(messageTosend),0))==-1  )
	{perror("failed to send message\n");return -1;}
	if((recvSize=recv(client_socket,messageReceive,1000,0)) ==-1){perror("failed to receive message");return -1;} // 
	
	// if username taken 
	char* nameTaken="ERR 00 USER NAME TAKEN";
	prepare_message(messageToCompare,nameTaken);
	if(strcmp(messageReceive,messageToCompare)==0){
		 return oldUserNameTakenOrNotExist(client_socket,messageReceive,messageToCompare);
	}
	//if username does not exist in the system 
	char* nameNotExist="ERR 01 USER NOT AVAILABLE";
	prepare_message(messageToCompare,nameNotExist);
	if(strcmp(messageToCompare,messageReceive)==0){
		return oldUserNameTakenOrNotExist(client_socket, messageReceive,messageToCompare);
	}
	// username not taken & username dose exist 
	char* auth="AUTH";
	strcpy(messageToCompare,nameNotTaken);
	prepare_message(messageToCompare, name);
	else if(strcmp(messageToCompare,messageReceive)==0){//HI NAME \n\r\n\r
		
	}
	else{
		printf("not the message I expected\n");
		return -10;
	}
}	
int oldUserNameNotTakenAndExist(){
	

}
int oldUserNameTakenOrNotExist(int client_socket, char messageReceive[100],char messageToCompare[100]){
	if((recvSize=recv(client_socket,messageReceive,1000,0)) ==-1){
		perror("failed to receive message");
		return -1;
	} 
	char* bye="BYE";
	prepare_message(messageToCompare,bye);
	if(strcmp(messageToCompare,messageReceive)!=0){
		perror("not the message I expected\n");
		return -10
	}
	else if(strcmp(messageToCompare,messageReceive)==0){
		printf("failed login, username already taken\n");
		//disconnect the communication here
		return 0; 
	}
	return 0;
}

int newUserlogin(){


}

void prepare_message(char*message_to_send,char* message){
	char * endNode="\r\n\r\n";	
	strcpy(message_to_send,message);
	strcat(message_to_send,endNode);
}


long int convert_portNumber(char* serverPort){
	char mid;
	char* endptr=&mid;
	errno =0;
	long int returnValue=strtol(serverPort, &endptr, 10);
	//underflow and overflow or no digits 
	if( (errno==ERANGE && (returnValue==LLONG_MAX || returnValue==LLONG_MIN)) || (errno==EINVAL && returnValue==0) ){
		printf("overflow/underflow happened or No digits was seen, no conversion is performed\n");
		return -1;
	}
	if(endptr==serverPort){
		printf("no digits was seen in the port number\n");
		return -1;
	}
	if (*endptr != '\0'){        /* Not necessarily an error... */
       printf("Further characters after number: %s\n", endptr);
		return -1;
	}
	return returnValue;
}