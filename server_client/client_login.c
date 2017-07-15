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
#include "client_login.h"
#include "utility.h"


int client_login(int client_socket,char*name, int cflags){
	//-1 disconnect -10 abnormal message from server 0 failed login 1 good 
	char messageReceive[1000];
	int continueValue=1;
	while(continueValue){
		continueValue=sendWOLFIE(client_socket,messageReceive); //on success return 1 
		continueValue=receiveELFLOW(client_socket); //on success return 1 
		
		if(cflags==1){
			continueValue=sendIAMnewName(client_socket,name);
			continueValue=receiveAfterIAMnewName(client_socket,messageReceive);
			if(ISuserNameTaken(messageReceive)){
				continueValue=oldUserNameTakenOrNotExist(client_socket); //can be used in new user case
			}
			else if(ISNewUserNameNotTaken(messageReceive,name)){
				continueValue=NewUserNameNotTakenAndExist(client_socket,name);
			}			
		}
		else if(cflags==0){
			continueValue=sendIAMNAME(client_socket,name); //on success return 1 
			continueValue=receiveAfterIAMNAME(client_socket, messageReceive);// on success return 1
			if ( ISuserNameTaken(messageReceive)){
				continueValue=oldUserNameTakenOrNotExist(client_socket); // -1 -10 0r 0
			}
			else if (ISuserNameNotExist(messageReceive)){
				continueValue=oldUserNameTakenOrNotExist(client_socket); //-1 -10 0r 0
			}
			else if (ISuserNameAuth(messageReceive,name)){
				continueValue=oldUserNameNotTakenAndExist(client_socket,name);
				if(continueValue==1){ //break out from the login in while loop
					break;
				}
			}
			else{
				continueValue=abnormalMessage();
				
			}
		}//cflags==0
	}//while loop
	return continueValue;
}	


int sendWOLFIE(int client_socket,char* messageReceive){
	int sendSize;
	char messageTosend[1000]="";
	char * wolfie="WOLFIE";
	prepare_message(messageTosend,wolfie);
	if((sendSize=send(client_socket,messageTosend,strlen(messageTosend),0))==-1){
		perror("failed to send message\n");
		return -1;
	}
	return readCharacter(client_socket,messageReceive);
}
int receiveELFLOW(int client_socket){
	char messageReceive[1000]="",messageToCompare[1000]="";
	char* elflow="ELFLOW";
	prepare_message(messageToCompare,elflow);
	if(strcmp(messageReceive,messageToCompare)!=0){
		perror("not the message I expected\n");
		return -10;
	}
	return 1;
}

int sendIAMnewName(int client_socket, char* name){
	int sendSize;
	char messageTosend[1000]="";
	char * endNode="\r\n\r\n";	
	char* Iamnew="IAMNEW";
	strcpy(messageTosend,Iamnew);
	strcat(messageTosend,name);
	strcat(messageTosend,endNode);
	if((sendSize=send(client_socket,messageTosend,strlen(messageTosend),0))==-1  )
	{perror("failed to send message\n");return -1;}
	return 1;
}
int receiveAfterIAMnewName(int client_socket,char* messageReceive){
	return readCharacter(client_socket,messageReceive);
}


int ISNewUserNameNotTaken(char* messageReceive,char* name){
	char* hi="HINEW";
	char* endNode="\r\n\r\n";
	char messageToCompare[1000];
	strcpy(messageToCompare,hi);
	strcat(messageToCompare,name);
	strcat(messageToCompare,endNode);
	if(strcmp(messageToCompare,messageReceive)==0){
		return 1;
	}
	else
		return -1;
}


int NewUserNameNotTakenAndExist(int client_socket,char* name){
	int sendSize;
	char messageTosend[100]="",messageReceive[1000]="",messageToCompare[1000]="";
	prepare_NEWPASS_message(messageTosend);
	if((sendSize=send(client_socket,messageTosend,strlen(messageTosend),0))==-1  )
		{perror("failed to send message\n");return -1;}
	/*if((recvSize=recv(client_socket,messageReceive,1000,0)) ==-1)
		{perror("failed to receive message");return -1;}*/
	if (readCharacter(client_socket, messageReceive)==-1) return -1;
	char* badPass="ERR 02 BAD PASSWORD";
	prepare_message(messageToCompare,badPass);
	if (strcmp(messageToCompare,messageReceive)==0){
			return oldUserNameNotValidPassWord(client_socket, messageToCompare, messageReceive);
	}
	char* goodPass="SSAPWEN";
	prepare_message(messageToCompare,goodPass);
	if (strcmp(messageToCompare,messageReceive)==0){
		return oldUserNameValidPassWord(client_socket,messageToCompare,messageReceive,name);
	}
	else{
		printf("not the message I expected\n");
		return -10;
	}
	return 1;
}


void prepare_NEWPASS_message(char* messageTosend){
		char passWord[1000]="0";
		char * endNode="\r\n\r\n";	
		printf("Enter the password: ");
		fgets(passWord,1000,stdin);
		int size_passWord=(int)strlen(passWord);
		passWord[size_passWord-1]='\0';
		char* pass="NEWPASS";
		strcpy(messageTosend, pass);
		strcat(messageTosend,passWord);
		strcat(messageTosend,endNode);
}

int sendIAMNAME(int client_socket,char*name){
	int sendSize, recvSize;
	char messageTosend[1000]="",messageReceive[1000]="",messageToCompare[1000]="";
	char * endNode="\r\n\r\n";	
	char* Iam="IAM";
	strcpy(messageTosend,Iam);
	strcat(messageTosend,name);
	strcat(messageTosend,endNode);
	if((sendSize=send(client_socket,messageTosend,strlen(messageTosend),0))==-1  )
	{perror("failed to send message\n");return -1;}
	return 1;
}
int receiveAfterIAMNAME(int client_socket, char* messageReceive){
	return readCharacter(client_socket, messageReceive);
}

int ISuserNameTaken(char* messageReceive){
	char* nameTaken="ERR 00 USER NAME TAKEN";
	char messageToCompare[1000];
	prepare_message(messageToCompare,nameTaken);
	if(strcmp(messageReceive,messageToCompare)==0){
		return 1;
	}
	return 0;
}

int ISuserNameNotExist(char*messageReceive){
	char* nameNotExist="ERR 01 USER NOT AVAILABLE";
	char messageToCompare[1000];
	prepare_message(messageToCompare,nameNotExist);
	if(strcmp(messageToCompare,messageReceive)==0){
		return 1;
	}
	return 0;
}
int ISuserNameAuth(char* messageReceive,char* name){
	char* auth="AUTH";
	char messageToCompare[1000];
	strcpy(messageToCompare,auth);
	strcat(messageToCompare,name);
	strcat(messageToCompare,"\r\n\r\n");
	//prepare_message(messageToCompare, name);
	if(strcmp(messageToCompare,messageReceive)==0){
		return 1;
	}	
	return 0;
}

int abnormalMessage(){
	printf("not the message I expected\n");
	return -10;
}




int oldUserNameTakenOrNotExist(int client_socket){
	char messageReceive[1000]="",messageToCompare[1000]="";
	if(readCharacter(client_socket, messageReceive)==-1)
		return -1;
	char* bye="BYE";
	prepare_message(messageToCompare,bye);
	if(strcmp(messageToCompare,messageReceive)!=0){
		perror("not the message I expected\n");
		return -10;
	}
	else if(strcmp(messageToCompare,messageReceive)==0){
		printf("failed login, username already taken\n");
		//disconnect the communication here
		return 0; 
	}
	return 0;
}
int oldUserNameNotTakenAndExist(int client_socket,char*name){
	int sendSize;
	char messageTosend[100]="",messageReceive[1000]="",messageToCompare[1000]="";
	prepare_PASS_message(messageTosend);
	if((sendSize=send(client_socket,messageTosend,strlen(messageTosend),0))==-1  )
		{perror("failed to send message\n");return -1;}
	/*if((recvSize=recv(client_socket,messageReceive,1000,0)) ==-1)
		{perror("failed to receive message");return -1;}*/
	if (readCharacter(client_socket, messageReceive)==-1) return -1;
	char* badPass="ERR 02 BAD PASSWORD";
	prepare_message(messageToCompare,badPass);
	if (strcmp(messageToCompare,messageReceive)==0){
			return oldUserNameNotValidPassWord(client_socket, messageToCompare, messageReceive);
	}
	char* goodPass="PASS";
	prepare_message(messageToCompare,goodPass);
	if (strcmp(messageToCompare,messageReceive)==0){
		return oldUserNameValidPassWord(client_socket,messageToCompare,messageReceive,name);
	}
	else{
		printf("not the message I expected\n");
		return -10;
	}
	return 1;
}
int oldUserNameValidPassWord(int client_socket, char*messageToCompare, char*messageReceive, char*name){
	if (readCharacter(client_socket, messageReceive)==-1) return -1;
	char* hi="HI";
	char * endNode="\r\n\r\n";	
	strcpy(messageToCompare, hi);
	strcat(messageToCompare,name);
	strcat(messageToCompare,endNode);
	if(strcmp(messageToCompare,messageReceive)!=0){
		perror("not the message I expected\n");
		return -10;
	}
	char* motd="MOTD";
	strcpy(messageToCompare,motd);
	if (strncmp(messageToCompare, messageReceive, 4)!=0){
		perror("not the message I expected\n");
		return -10;
	}
	else if (strncmp(messageToCompare, messageReceive,4)==0){
		printf("successfully login in now !!!!\n");
		return 1;  
	}
	
}
int oldUserNameNotValidPassWord(int client_socket, char*messageToCompare, char* messageReceive){
	if(readCharacter(client_socket, messageReceive)==-1) return -1;
	char* bye="BYE";
	prepare_message(messageToCompare,bye);
	if(strcmp(messageToCompare,messageReceive)!=0){
		perror("not the message I expected\n");
		return -10;
	}
	else if(strcmp(messageToCompare,messageReceive)==0){
		printf("failed login, password not valid \n");
		//disconnect the communication here
		return 0; 
	}
	return 0;
}


void prepare_PASS_message(char* messageTosend){
		char passWord[1000]="0";
		char * endNode="\r\n\r\n";	
		printf("Enter the password: ");
		fgets(passWord,1000,stdin);
		int size_passWord=(int)strlen(passWord);
		passWord[size_passWord-1]='\0';
		char* pass="PASS";
		strcpy(messageTosend, pass);
		strcat(messageTosend,passWord);
		strcat(messageTosend,endNode);
}


void prepare_message(char*message_to_send,char* message){
	char * endNode="\r\n\r\n";	
	strcpy(message_to_send,message);
	strcat(message_to_send,endNode);
}




