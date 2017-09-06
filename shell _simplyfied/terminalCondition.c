#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<terminos.h>


void print_terminal(struct termios* term){
	//check c_iflag;
	if(term->c_lflag & ISIG){
		printf("ISIG is set\n");
	}
	if(term->c_lflag & ECHO){
		printf("ECHO is set\n");
	}
	if(term->c_lflag & ECHOE){
		printf("ECHOE is set\n");
	}
	if(term->c_iflag & ECHOK{
		printf("ECHOK is set\n");
	}
	if(term->c_iflag & ECHONL){
		printf("ECHONL is set\n");
	}
	  if(term->c_iflag & ICANON){
		printf("ICANON is set\n");
	}
}