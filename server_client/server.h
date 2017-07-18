#ifndef SERVER_H
#define SERVER_H

void * thread_accept(void* vargp);
struct acceptThreadArgs{
		int welcomeSocket;
		int communicateSocket;
	
};
void * thread_login(void* vargp);

long int convert_portNumber(char* serverPort);
#endif 
