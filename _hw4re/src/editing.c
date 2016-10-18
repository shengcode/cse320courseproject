#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "arraylist_struct.h"
#include "process.h"
void prompt(char**envp){
	ArrayList* history = initialize_arraylist(sizeof(char*));
	size_t n=1024;
	char* buffer=malloc(n);
	while(1){
		getline(&buffer, &n, stdin);
		int sizeofstring= strlen(buffer);
		printf("the number of characters you just read in is %d\n",sizeofstring);
		ArrayList*currentbuffer= initialize_arraylist(sizeof(char));
		int i;
		for(i=0;i<sizeofstring-1; i++){
			add_element(currentbuffer,buffer+i);
		}
		char nullPointer='\0';
		add_element(currentbuffer,&nullPointer);
		add_element(history, (char**)(&(currentbuffer->data)));
		printf("all the things you have just input are:\n");
		int h;
		for(h=0;h<history->size;h++){
			printf("%s\n",(char*)(((ArrayList*)get_ith_element(history,h))->data));
		}
		process(currentbuffer->data, envp);
		freeArraylist(currentbuffer);
		
	}
	
    freeArraylist(history);
	free(buffer);
}