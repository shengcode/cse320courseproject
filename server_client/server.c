#define _GNU_SOURCE   
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>
#include<errno.h>          
#include <pthread.h>
#include <stdlib.h>
#include "server.h"
#include "server_login.h"
#include "array_list.h"

int main(int argc, char**argv){
	int welcomeSocket;	
	struct sockaddr_in SA;
	pthread_t tid;
	fd_set readfds;
	int number_of_success_login=0;
	
	
	
	
	welcomeSocket=socket(AF_INET,SOCK_STREAM,0);
		if(welcomeSocket==-1) {perror("failed to create a good socket\n"); return 1;}	
	memset((void*)&SA,0,sizeof(SA));
	const char src[20]="127.0.0.1";	
	
	SA.sin_family=AF_INET;	//initialize structure
	SA.sin_port=htons(12000);	
	inet_pton(AF_INET,src,&(SA.sin_addr.s_addr));	
	if(bind(welcomeSocket,(struct sockaddr*)&SA,sizeof(struct sockaddr_in))==-1){
		perror("failed to bind the server\n");
		return 1;
	}
	else 
		printf("successfully bind\n");
	if(listen(welcomeSocket,1024)==-1){
		perror("failed to listen\n");
		return 1;
	}
	else
		printf("now listening\n");
	
	//create accept thread
	struct acceptThreadArgs actThreadArg;
	actThreadArg.welcomeSocket=welcomeSocket;
	actThreadArg.communicateSocket=-1;	
	pthread_create(&tid, NULL,thread_accept,(void*)(&actThreadArg));
	pthread_setname_np(tid,"ACCEPT THREAD");
	pthread_join(tid,NULL);
	return 1;
}

void * thread_accept(void* vargp){
	struct acceptThreadArgs* actThreadArg = (struct acceptThreadArgs*) vargp;
	int welcomeSocket=actThreadArg->welcomeSocket;
	struct sockaddr_in SA;
	pthread_t tid;
	socklen_t socketLength=sizeof(struct sockaddr);
	while(1){
		if((actThreadArg->communicateSocket=accept(welcomeSocket,(struct sockaddr*)&SA,&socketLength))==-1){
			perror("failed to accept the connection from client\n");
			exit(EXIT_FAILURE);
		}
		printf("the communicate socket is %d\n",actThreadArg->communicateSocket);
		pthread_create(&tid,NULL,thread_login,&(actThreadArg->communicateSocket));
		pthread_setname_np(tid,"LOGIN THREAD");
		puts("connection accepted\n");
		//pthread_join(tid,NULL);
		
	}
	

}

void * thread_login(void* vargp){
	int communicateSocket=*((int*) vargp);
	printf("HELO\n");
	
	while(1){
		Arraylist readSentence;
		init_arraylist(&readSentence, sizeof(char));
		if (readCharacter(communicateSocket,&readSentence)==0){
			printf("failed to receive message\n");
			break;
		}
		char sendMessage[1000]="0";
		char null_ter='\0';
		append(&readSentence, &null_ter);
		char* des=strcpy(sendMessage,(char*)readSentence.data);
		printf("the des string is %s\n",des);
		printf("the message is %s\n",sendMessage);
		send(communicateSocket,sendMessage,1000,0);
		freeArrayList(&readSentence);
	}
}



