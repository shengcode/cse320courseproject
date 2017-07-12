#define _GNU_SOURCE   
#include<stdio.h> 
#include <unistd.h>
#include "server_login.h"
#include "array_list.h"









int readCharacter(int fd, Arraylist* readCharacter){
	// 0 false, 1 true
	char buf[1];
	
	char endChars[5]="\r\n\r\n";
	int endProgress=0;
	while(1){
		buf[0]='0';
		int readReturn=-2;
		printf("1, readReturn is %d\n",readReturn);
		readReturn=read(fd, buf, 1);
		printf("2, readReturn is %d\n",readReturn);
		printf("the char I accepted is %c\n",buf[0]);
		if(readReturn<0){
			//freeArrayList(readCharacter);
			return 0;
		}
		else if(readReturn==0){
			continue;
		}
		else if(readReturn==1){			
			append(readCharacter, (void*)&buf);
			if(buf[0]==endChars[endProgress]) {
				endProgress=endProgress+1; 
			}
			else{
				endProgress=0;
			}
			printf("endprogress value is %d\n",endProgress);
			if(endProgress==4) 
				return 1;
			
		}
		
	}

}