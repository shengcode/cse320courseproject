void * thread_login(void* vargp){
	int communicateSocket=*((int*) vargp);
	char messageReceive[1000]="";
	char name[100];
	char password[100];
	int continueValue=1;
	while(1){
		continueValue=receiveWOLFIE(communicateSocket, messageReceive);
		continueValue=sendEIFLOWandReceive(communicateSocket,messageReceive);
		if (ISnewUser(messageReceive, name)){
			if(!ISnameExist(name)){
				continueValue=sendHinewName(communicateSocket, name);
				continueValue=receiveNewPass(communicateSocket,messageReceive,password);
				if(ISvalidPassword(password)){
					continueValue=sendSSAPWENandHi(communicateSocket);
					// login successfully
				}
				if(!ISvalidPassword(password)){
					continueValue=sendErr02Bye(communicateSocket);
					//disconnected
				}
			}
			if(ISnameExist(name)){
				continueValue= sendErr00Bye(int communicateSocket);	
				//disconnected;
			}
		}
		else if (ISoldUser(messageReceive, name)){
			if(!ISnameExist(name)){
				//old user dose not exist 
				continueValue=sendErr01Bye(communicateSocket);
				//disconnect here
				
			}
			else if(ISnameExist(name)){
				//old user dose exist
				continueValue=sendAuth(communicateSocket, name);
				continueValue=receivePASS(communicateSocket,messageReceive,password);
				if(IScorrectPassword(name,password)){
					continueValue=sendSSAPandHi(communicateSocket,name);
					//login successfully
				}
				if(!IScorrectPassword(name, password)){
					continueValue=sendErr02Bye(communicateSocket);
					//disconnected
				}
			}
		}
		else{
			returnValue=-10;
			perror("not New User, not Old User, not expected message\n");
			
		}
	}
}

int receiveWOLFIE(int communicateSocket,char* messageReceive){
	char messageToCompare[1000]="WOLFIE";
	char* endNode="\r\n\r\n";
	strcat(messageToCompare, endNode);
	if (readCharacter(communicateSocket,messageReceive)==0){
			perror("failed to receive message\n");
			return -1;
	}
	if(strcmp(messageToCompare, messageReceive)!=0){
		perror("not WOLFIE as I expected\n");
		return -10;
	}
	return 1;	
}

int sendEIFLOWandReceive(int communicateSocket,char*messageReceive){
	char messageToSend[1000]="ELFLOW";
	char* endNode="\r\n\r\n";
	int sendSize;
	strcat(messageToSend, endNode);
	if((sendSize=send(communicateSocket,messageTosend,strlen(messageTosend),0))==-1){
		perror("failed to send EIFLOW message\n");
		return -1;
	}
	if (readCharacter(communicateSocket,messageReceive)==0){
			perror("failed to receive IAM or IAMNEW message\n");
			return -1;
	}
	
	return 1;
}

int ISnewUser(char* messageReceive, char*name){
	char* messageToCompare="IAMNEW";
	if(strcmp(messageReceive,messageToCompare,7)==0){
		messageReceive[strlen(messageReceive)-4]='\0';
		strcpy(name,(messageReceive+7));
		return 1;
	}	
	return 0;	
}

int ISoldUser(char* messageReceive, char*name){
	char* messageToCompare="IAM";
	if(strcmp(messageReceiv,messageToCompare,3)==0){
		messageReceive[strlen(messageReceive)-4]='\0';
		strcpy(name, (messageReceive+3));
		return 1;
	}
	return 0;
}

int ISnameExist(char*name){
	return 1; //todo 
}

int sendErr00Bye(int communicateSocket){
	char messageToSend[1000]="ERR 00 USERNAME TAKEN";
	char* endNode="\r\n\r\n";
	strcat(messageToSend, endNode);
	if((sendSize=send(communicateSocket,messageToSend,strlen(messageTosend),0))==-1){
		perror("failed to send ERR 00 message\n");
		return -1;
	}
	char* bye="BYE";
	strcpy(messageToSend,bye);
	strcat(messageToSend, endNode);
	if((sendSize=send(communicateSocket,messageToSend,strlen(messageTosend),0))==-1){
		perror("failed to send BYE message\n");
		return -1;
	}
	return 1;
}

int sendHinewName(int communicateSocket, char* name){
	char messageToSend[1000]="HINEW"
	int sendSize;
	strcat(messageToSend, name);
	strcat(messageToSend,"\r\n\r\n");
	if((sendSize=send(communicateSocket,messageToSend,strlen(messageTosend),0))==-1){
		perror("failed to send HINEW message\n");
		return -1;
	}
	return 1;
}

int receiveNewPass(int communicateSocket, char* messageReceive,char*password){
	char messageToCompare[1000]="NEWPASS";
	if (readCharacter(communicateSocket,messageReceive)==0){
			perror("failed to receive message\n");
			return -1;
	}
	if (strcmp(messageToCompare,messageReceive,7)==0){
		messageReceive[strlen(messageReceive)-4]='\0';
		strcpy(password,messageReceive);
		return 1;
	}
	perror("not the message I expected\n");
	return -10;
}
int ISvalidPassword(char* password){

	//return 0;
	return 1;
}

int sendSSAPWENandHi(int communicateSocket,char* name){
	char messageToSend[1000]="SSAPWEN";
	strcat(messageToSend,"\r\n\r\n");
	int sendSize;
	if((sendSize=send(communicateSocket,messageToSend,strlen(messageTosend),0))==-1){
		perror("failed to send SSAPWEN message\n");
		return -1;
	}
	char* hi="HI";
	strcpy(messageToSend,hi);
	strcat(messageToSend,name);
	strcat(messageToSend,"\r\n\r\n");
	if((sendSize=send(communicateSocket,messageToSend,strlen(messageTosend),0))==-1){
		perror("failed to send hi name message\n");
		return -1;
	}
	return 1;
}

int sendErr02Bye(int communicateSocket){
	char messageToSend[1000]="ERR 02 BAD PASSWORD";
	char* endNode="\r\n\r\n";
	strcat(messageToSend, endNode);
	if((sendSize=send(communicateSocket,messageToSend,strlen(messageTosend),0))==-1){
		perror("failed to send ERR 02 message\n");
		return -1;
	}
	char* bye="BYE";
	strcpy(messageToSend,bye);
	strcat(messageToSend, endNode);
	if((sendSize=send(communicateSocket,messageToSend,strlen(messageTosend),0))==-1){
		perror("failed to send BYE message\n");
		return -1;
	}
	return 1;
}
int sendAuth(int communicateSocket, char*name){
	char messageToSend[1000]="AUTH";
	strcat(messageToSend,name);
	strcat(messageToSend,"\r\n\r\n");
	int sendSize;
	if((sendSize=send(communicateSocket,messageToSend,strlen(messageTosend),0))==-1){
		perror("failed to send AUTH message\n");
		return -1;
	}
	return 1;
}

int receivePASS(int communicateSocket,char* messageReceive,char*password){
	char messageToCompare[1000]="PASS";
	if (readCharacter(communicateSocket,messageReceive)==0){
			perror("failed to receive PASS message\n");
			return -1;
	}
	if(strcmp(messageToCompare,messageReceive,4)==0){
		messageReceive[strlen(messageReceive)-4]=='\0';
		strcpy(password,messageReceive);
		return 1;
	}
	perror("not the message I expeceted\n");
	return -10;
}

int IScorrectPassword(char* name, char* password){
	// open the file and check if the file match the password

}

int sendSSAPandHi(int communicateSocket,char* name){
	char messageToSend[1000]="SSAP";
	strcat(messageToSend,"\r\n\r\n");
	int sendSize;
	if((sendSize=send(communicateSocket,messageToSend,strlen(messageTosend),0))==-1){
		perror("failed to send SSAP message\n");
		return -1;
	}
	char* hi="HI";
	strcpy(messageToSend,hi);
	strcat(messageToSend,name);
	strcat(messageToSend,"\r\n\r\n");
	if((sendSize=send(communicateSocket,messageToSend,strlen(messageTosend),0))==-1){
		perror("failed to send hi name message\n");
		return -1;
	}
	return 1;
}

int sendErr01Bye(int communicateSocket){
	char messageToSend[1000]="ERR 01 USER NOT AVAILABLE";
	char* endNode="\r\n\r\n";
	strcat(messageToSend, endNode);
	if((sendSize=send(communicateSocket,messageToSend,strlen(messageTosend),0))==-1){
		perror("failed to send ERR 01 message\n");
		return -1;
	}
	char* bye="BYE";
	strcpy(messageToSend,bye);
	strcat(messageToSend, endNode);
	if((sendSize=send(communicateSocket,messageToSend,strlen(messageTosend),0))==-1){
		perror("failed to send BYE message\n");
		return -1;
	}
	return 1;
}
