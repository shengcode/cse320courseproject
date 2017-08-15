#ifndef SERVER_H
#define SERVER_H

void * thread_accept(void* vargp);
struct acceptThreadArgs{
		int welcomeSocket;
		int communicateSocket;
		char MOTD[100];
		char accountFile[100];
	
};
void * thread_login(void* vargp);

long int convert_portNumber(char* serverPort);

int setUpDatabase();
int createTable();

#endif 
