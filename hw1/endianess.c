#include<stdio.h>
#include<stdlib.h>

int main(int argc, char*argv[]){
	unsigned int i =1;
	char *c = (char*)&i;
	if(*c){
		printf("little endian\n");
	}else{
		printf("big endian\n");

	}
	return 	EXIT_SUCCESS;
}
