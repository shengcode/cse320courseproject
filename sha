#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"main.h"
#include "array_list.h"
#include "buildProcess.h"


int main (int argc, char ** argv, char **envp) {       
	
	size_t n=100;
	char* lineptr=NULL;
	size_t number_characters=0;
	lineptr=(char*) malloc(n*100);	
	
	while(1){		
		printf("320sh>");
		number_characters=getline(&lineptr, &n, stdin);
		Arraylist stdinInput;
		init_arraylist(&stdinInput, sizeof(char));
		addTheStinToArrayList(lineptr,number_characters,&stdinInput); //inludes the '\n' character;
		printf("now the commandline reading in is null terminatored %s",(char*)(&stdinInput)->data);
		BuildProcess((char*)((&stdinInput)->data),envp);
		//printCharPointArraylist(&stdinInput);
		//InQuotationReplacement(&stdinInput);
		//printCharArraylist(&stdinInput);
		//tokenizeProcessStr((char*)( (&stdinInput)->data));
	}
}

void addTheStinToArrayList(char*lineptr,int number_characters,Arraylist * stdinInput){
	for(int i=0;i<number_characters;i++){
		append(stdinInput, lineptr+i);
	}
	char nullTerminator='\0';
	append(stdinInput,&nullTerminator);
}


void tokenizeProcessStr(char* procStr){ //based on space & ' 
	char* strtokPtr;
	char* token;
	do{
		token = strtok_r(procStr, " \"'", &strtokPtr);
		if(token == NULL) break;
		printf("the token I got is %s\n",token);
		procStr = NULL;		
		
	}while(1);
	
}










