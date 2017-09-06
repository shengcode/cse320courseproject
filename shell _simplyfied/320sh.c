#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<termios.h>
#include<unistd.h>
#include"main.h"
#include "buildProcess.h"


int main (int argc, char ** argv, char **envp) {      
	struct termios old_term;
	if(tcgetattr(STDIN_FILENO, &old_term)!=0)
		perror("tcgetattr() error");
	printf("before change the default\n");
	print_terminal(&old_term);
	old_term.c_lflag |=ISIG; 
	//old_term.c_lflag &=~(ECHO|ECHOE|ECHOK|ECHONL|ICANON);
	old_term.c_lflag=ECHO | old_term.c_lflag;
	if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&old_term)<0)
		perror("failed to setup terminal\n");
	printf("after change the default\n");
	print_terminal(&old_term);
	
	
	while(1){	
		char* command=malloc(1094*sizeof(char));
		size_t n_char=1094;
		printf("320sh>");
		getline(&command, &n_char, stdin);
		//command[]
		ProcessCommand(command,envp);
		free(command);
	}
}






void print_terminal(struct termios* term){
	if(term->c_lflag & ISIG){
		printf("ISIG is set\n");
	}
	if(term->c_lflag & ECHO){
		printf("ECHO is set\n");
	}
	if(term->c_lflag & ECHOE){
		printf("ECHOE is set\n");
	}
	if(term->c_iflag & ECHOK){
		printf("ECHOK is set\n");
	}
	if(term->c_iflag & ECHONL){
		printf("ECHONL is set\n");
	}
	  if(term->c_iflag & ICANON){
		printf("ICANON is set\n");
	}
}




