#define _GNU_SOURCE   
#include<stdio.h> 
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "server_login.h"
#include "array_list.h"


void * thread_login(void* vargp){
	int communicateSocket=*((int*) vargp);
	while(1){
		Arraylist readSentence;
		init_arraylist(&readSentence, sizeof(char));
		if (readCharacter(communicateSocket,&readSentence)==0){
			printf("failed to receive message\n");
			break;
		}
		char sendMessage[1000]="0";
		char null_ter='\0'; // the message I send is null-terminated
		append(&readSentence, &null_ter);
		char* des=strcpy(sendMessage,(char*)readSentence.data);
		int sendSize=strlen(sendMessage);
		send(communicateSocket,sendMessage,sendSize,0);
		freeArrayList(&readSentence);
	}
}




















int readCharacter(int fd, Arraylist* readCharacter){
	// 0 false, 1 true
	char buf[1];	
	char endChars[5]="\r\n\r\n";
	int endProgress=0;
	while(1){
		buf[0]='0';
		int readReturn=-2;
		readReturn=read(fd, buf, 1);
		if(readReturn<0){
			return 0;
		}
		else if(readReturn==0){
			continue;
		}
		else if(readReturn==1){			
			append(readCharacter, (void*)&buf);
			if(buf[0]==endChars[endProgress]) {
				endProgress=endProgress+1; 
			}
			else{
				endProgress=0;
			}			
			if(endProgress==4) {
							
				return 1;
			}
		}
		
	}

}