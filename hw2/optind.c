#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>

int main (int argc, char**argv){
	int opt;
	int vflags=0;
	char output_format[15];
	char input_file[50];
	char output_file[50];
	while( (opt = getopt(argc, argv, "hve:a:"))!=-1){
		switch(opt){
		case 'h':
			printf("this is -h\n");
			break;
		case 'v':
			vflags++;
			printf("there a %d vflags\n",vflags);
			break;
		case 'e':
			printf("this is -e flag\n");
			printf("the argument following -e is %s\n",optarg);
			strncpy(output_format,optarg,15);
			break;
		case 'a':
			printf("this ia -a flag\n");
			printf("the argument following -a is %s\n",optarg);
			break;
		case '?':
			break;
		default:
			printf("not a right argument\n");
			exit(EXIT_FAILURE);
		}
		printf("optind is %d\n", optind);
		printf("my argc is %d\n",argc);
	   	//strcpy(input_file,argv[optind]);
		//strcpy(output_file,argv[++optind]);
		
		
	}
}