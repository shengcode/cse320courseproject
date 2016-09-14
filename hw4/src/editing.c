#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "arraylist_struct.h"

void prompt(){
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
		char newline= '\0';
		add_element(currentbuffer,&newline); // at the end add the null terminator 
		add_element(history, currentbuffer->data);
		printf("all the things you have just input are:\n");
		int h;
		for(h=0;h<history->size;h++){
			printf("%s\n",(char*)get_ith_element(history,h));
		}
	}
	free(buffer);
}