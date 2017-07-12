#define _GNU_SOURCE
#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<string.h>


int main(int argc, char *argv[]){
	int opt;
	int vflag=-1;
	int hflag=-1;
	int port_number;
	char MOTO[100];
	while ((opt = getopt(argc, argv, "hv")) != -1) {
		switch (opt) {
			case 'h':
				vflag=1;
				printf("print the help menu\n");
				break;
			case 'v':
				vflag=1;
				printf("print the verbose\n");
				
			default: 
				printf("this is default\n");
               }
           }  
	if(argv[optind]==NULL){
		printf("need port_number and MOTO\n");
		return (EXIT_FAILURE);
	}
	else {
		port_number=atoi(argv[optind]);
		printf("the port number you entered is %d\n",port_number);
		if(port_number==0) {printf("need a digital number as port_number\n");return (EXIT_FAILURE);}
	}
	if(argv[optind+1]==NULL){
		printf("need MOTO\n");
		return(EXIT_FAILURE);
	}
	else{
		strcpy(MOTO,argv[optind+1]);
	}
	return (EXIT_SUCCESS);
}




