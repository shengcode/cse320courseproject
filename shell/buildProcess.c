#include<stdio.h>
#include <sys/types.h>
#include"buildProcess.h"
#include"main.h"
#include "array_list.h"



void InQuotationReplacement(Arraylist * stdinInput){
	int insideQuotation=0;
	for(int i=0;i<stdinInput->size;i++){
		char inputChar=*((char*)(stdinInput->data)+i);
		if(inputChar=='"' || inputChar=="\"") insideQuotation=1;
		if(insideQuotation==1 && inputChar==' '){ * ((char*) (stdinInput->data)+i)=127;}  
		else if(insideQuotation==1 && inputChar=='|'){* ((char*) (stdinInput->data)+i)=17;}
		else if(insideQuotation==1 && inputChar=='<'){* ((char*) (stdinInput->data)+i)=18;}
		else if(insideQuotation==1 && inputChar=='>'){* ((char*) (stdinInput->data)+i)=19;}
	}
}


void 