#define _GNU_SOURCE   
#include<stdio.h> 
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "server_login.h"
#include "array_list.h"
#include "utility.h"

void * thread_login(void* vargp){
	int communicateSocket=*((int*) vargp);
	while(1){
		Arraylist readSentence;
		init_arraylist(&readSentence, sizeof(char));
		if (readCharacter(communicateSocket,&readSentence)==-1){
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
























