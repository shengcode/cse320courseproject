#include<stdio.h>
#include<stdlib.h>
#include"main.h"
#include"file2.h"
#include"file3.h"
#include "array_list.h"

int main (int argc, char ** argv, char **envp) {       
	function2();
	size_t n=100;
	char* lineptr=NULL;
	size_t number_characters=0;
	lineptr=(char*) malloc(n*100);	
	
	while(1){
		
		printf("320sh>");
		number_characters=getline(&lineptr, &n, stdin);
		//printf("the number of characters read is %d\n",(int)number_characters);
		//printf("the line read in is %s\n",lineptr);	
		Arraylist stdinInput;
		init_arraylist(&stdinInput, sizeof(char));
		addTheStinToArrayList(lineptr,number_characters,stdinInput);
		printCharArraylist(&stdinInput);
		printf("hi there\n");
	}
}

void addTheStinToArrayList(char*lineptr,int number_characters,Arraylist stdinInput){
	for(int i=0;i<number_characters;i++){
		printf("the char you are adding is %c\n",*(lineptr+i));
		append(&stdinInput, lineptr+i);
	}
	printCharArraylist(&stdinInput);
}