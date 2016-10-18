#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "arraylist_struct.h"
#include "process.h"

int process(char*command, char**env){
	char* commandsaver = malloc(strlen(command)+1);
	strcpy(commandsaver,command);
	quotationReplace(command);
	ArrayList* processTokenArray = initialize_arraylist(sizeof(char*));
	getProcessToken(command,processTokenArray);	
	int i;
	for(i=0;i<processTokenArray->size; i++){
		printf("the different process are %s\n",(char*)get_ith_element(processTokenArray, i));
	}
	for(i=0; i<processTokenArray->size;i++){
		char* processToken = (char*)get_ith_element(processTokenArray,i);
		ArrayList*subProcessTokenArray = getSubProcessToken(processToken);
		for(i=0;i<subProcessTokenArray->size;i++){
			printf("the different subproceses are %s\n",(char*)get_ith_element(subProcessTokenArray,i));
		}
	}	
	free(commandsaver);
	return 0;
}

void quotationReplace(char*command){
	int i;
	int inQuotation=0;
	for(i=0;command[i]!='\0';i++){
		if(command[i]=='"' || command[i]=='/') inQuotation=!inQuotation;
			if(inQuotation && command[i]==' ') command[i]=127; // replace space with DEL
			else if(inQuotation && command[i] == '<') command[i]=17; // device control 1
			else if(inQuotation && command[i] == '>') command[i]=18; //device control 2
			else if(inQuotation && command[i] == '|') command[i]=19; //device control 3
	}
}
void quotationReturn (char* command){
	int i;
	for(i=0; command[i]!='\0'; i++){
		if(command[i]==127) command[i]=' ';
		else if(command[i]==17) command[i]='<';
		else if(command[i]==18) command[i]='>';
		else if(command[i]==19) command[i]='|';
	}
}

void getProcessToken(char*command, ArrayList*processTokenArray){
	char* str1;
	char* token;
	char* tokensaver1;
	const char* delim="|";
	for(str1=command; ; str1=NULL){
		token = strtok_r(str1,delim, &tokensaver1);
		if(token==NULL) break;
		add_element(processTokenArray,token);		
	}
}

ArrayList* getSubProcessToken(char*processStr){
	char* strtokPtr;
	ArrayList* subProcessTokenArray = initialize_arraylist(sizeof(char*));
	char* token;
	do{
		token=strtok_r(processStr," \"'",&strtokPtr);
		if(token==NULL) break;
		processStr=NULL;
		char* tokenCpy=malloc(sizeof(char)*strlen(token)+1);
		strcpy(tokenCpy,token);
		add_element(subProcessTokenArray,tokenCpy);
		free(tokenCpy);
		if(token==NULL) break;
	}while(1);
	return subProcessTokenArray;	
}