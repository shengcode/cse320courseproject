#ifndef MAIN_H
#define MAIN_H
#include "array_list.h"
int main (int argc, char ** argv, char ** envp);
void addTheStinToArrayList(char*lineptr,int number_characters,Arraylist* stdinInput);
void tokenizeProcessStr(char* procStr);
#endif