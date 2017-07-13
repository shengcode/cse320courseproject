#ifndef SERVER_LOGIN_H
#define SERVER_LOGIN_H

#include "array_list.h"
void * thread_login(void* vargp);
int readCharacter(int fd, Arraylist* readCharacter);

#endif