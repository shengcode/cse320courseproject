#include<stdio.h>
#include<stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "arraylist_struct.h"
#include "editing.h"
#include "process.h"

static struct termios oldTerm, defaultTerm;
int main(int argc, char**argv,char**envp){
	tcgetattr(STDIN_FILENO, &oldTerm);
	defaultTerm = oldTerm;
	defaultTerm.c_lflag |= ISIG;
	defaultTerm.c_lflag |= (ECHO|ECHOE|ECHOK|ECHONL|ICANON);
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &defaultTerm) <0 ){
		printf("fail to setup terminal environment \n");
	}
	//prompt();
	//char*command = "abd | abc|d|efgh";
	//char command[100] = "abd | abc|d|efgh";
	
	//process(command, envp);
	prompt(envp);
}
