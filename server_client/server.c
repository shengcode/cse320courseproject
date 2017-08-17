#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>          
#include <stdlib.h>
#include <limits.h>
#include <sqlite3.h>
#include "server.h"
#include "init_server.h"
#include "server_login.h"
#include "array_list.h"

int main(int argc, char**argv){
	
	int vflags;
	char portNumber[100];
	char MOTD[1000];
	char accountFile[1000];
	if(init_server( argc, argv,&vflags, portNumber, MOTD,accountFile)==0){
		exit(EXIT_FAILURE);
	}	
	long int converted_portNumber;
	if ( (converted_portNumber=convert_portNumber(portNumber))==-1){exit(EXIT_FAILURE);}
	printf("the port number is %ld\n",converted_portNumber);
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
	//SA.sin_port=htons(12000);	
	SA.sin_port=htons((uint16_t) converted_portNumber);
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
	strcpy(actThreadArg.MOTD,MOTD);
	strcpy(actThreadArg.accountFile,accountFile);
	
	sqlite3 *db;
	// set up datebase 
	if(setUpDatabase(db)==0) 
		return 0;
	
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
		//pthread_create(&tid,NULL,thread_login,&(actThreadArg->communicateSocket));
		pthread_create(&tid,NULL,thread_login,vargp);
		pthread_setname_np(tid,"LOGIN THREAD");
		puts("connection accepted\n");
		//pthread_join(tid,NULL);
		
	}
	

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

int setUpDatabase(sqlite3 *db){
	// try to connect to database 
	
	char *zErrMsg=0;
	int rc;
	char* sql;	
	rc=sqlite3_open_v2("userInfoDB.db",&db,SQLITE_OPEN_READWRITE,NULL); // open the database when it exists
	if(rc){
		fprintf(stderr,"There is NON existent database\n");
		rc=sqlite3_open("userInfoDB.db",&db); // create a database 
		if(rc){
			fprintf(stderr,"can't Create database: %s\n",sqlite3_errmsg(db));
			return 0;
	     } 
		else {
      		fprintf(stdout, "create New database successfully\n");
			// create table 
			if(createTable(db)==1){
				sqlite3_close(db);
				return 1;
			}
			else if(createTable(db)==0){
				sqlite3_close(db);
				return 0;
			}
   		}
	}
	fprintf(stderr,"Opened existent database successfully\n");
	sqlite3_close(db);
	return 1;
}
static int createTable_callback(void*NotUser, int argc, char**argv,char**azColName){
	int i;
	for(i=0;i<argc;i++){
		printf("%s = %s\n",azColName[i],argv[i]? argv[i]:"NULL");
		printf("\n");
		return 0;
	}
  
}

int createTable(sqlite3 *db){
	//sqlite3 *db;
	char *zErrMsg=0;
	int rc;
	char* sql;
	//SQL statement
	sql = "CREATE TABLE USER_INFO("  \
         "USER_NAME TEXT PRIMARY KEY     NOT NULL," \
         "PASSWORD  TEXT );";


   	/* Execute SQL statement */
   	rc = sqlite3_exec(db, sql, createTable_callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
		fprintf(stderr,"the program failed create table");
   		fprintf(stderr, "SQL error: %s\n", zErrMsg);
      	sqlite3_free(zErrMsg);
		return 0; //failed created table
   	} 
	fprintf(stdout, "Table created successfully\n");
	return 1;
}