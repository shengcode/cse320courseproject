#include<stdio.h>
#include<stdlib.h>
#include<string.h>



int main (int argc, char ** argv, char **envp) {      
	while(1){	
		char* command=malloc(1094*sizeof(char));
		size_t n_char=1094;
		printf("320sh>");
		getline(&command, &n_char, stdin);
		printf("%s",command);
		free(command);
	}
}
