#include<stdio.h>
#include <sys/types.h>
#include<stdlib.h>
#include<string.h>
#include"buildProcess.h"
#include"main.h"
#include "array_list.h"

int BuildProcess(char*command, char**env){
		char* commdlineBackup=(char*)malloc(strlen(command)+1);
		strcpy(commdlineBackup,command);
		InQuotationReplacement(command);
		Arraylist tokenizeWithPipe;
		init_arraylist(&tokenizeWithPipe, sizeof(char*));
		TokenizeCommandStr(command,&tokenizeWithPipe);
		Process* lastProc = NULL;
		Process* firstProc = NULL;
		int isBackground = 0;
	 	int i;
		for(i=1;i<=(&tokenizeWithPipe)->size;i++){
			char* processStr=*(char** )getIthElement(&tokenizeWithPipe,i);
			printf("my %dth processStr is %s\n", i, processStr);
			Arraylist tokenizeWithSpace_Quote;
			init_arraylist(&tokenizeWithSpace_Quote,sizeof(char*));
			TokenizeProcessStr(processStr,&tokenizeWithSpace_Quote);
			printCharPointArraylist(&tokenizeWithSpace_Quote);
		}
	
	
	
	return 1;
}




void InQuotationReplacement(char * command){
	int insideQuotation=0;
	int i;
	for(int i=0;command[i]!='\0';i++){
		if(command[i]=='"' || command[i]=='\"') insideQuotation=1;
		if(insideQuotation==1 && command[i]==' '){ command[i]=127;}  
		else if(insideQuotation==1 && command[i]=='|'){command[i]=17;}
		else if(insideQuotation==1 && command[i]=='<'){command[i]=18;}
		else if(insideQuotation==1 && command[i]=='>'){command[i]=19;}
	}
}
 
void TokenizeCommandStr(char* command,Arraylist* tokenizeWithPipe){
	char *token;
	char* saveptr;
	while(1){
		token=strtok_r(command,"|",&saveptr);
		command=NULL;
		if (token==NULL) break;
		append(tokenizeWithPipe, (void*)&token);
		//printf("my token is %s\n", token);
	}
}
void TokenizeProcessStr(char* processStr,Arraylist* tokenizeWithSpace_Quote){
	char* token;
	char* saveptr;
	while(1){
		token=strtok_r(processStr," \"'",&saveptr);
		processStr=NULL;
		if(token==NULL) break;
		append(tokenizeWithSpace_Quote,(void*)&token);
		printf("my token is %s\n",token);
	}
}


TokenRediInfo tokenRedirectionInfo(char* token){
	TokenRediInfo result;
	result.direction = 0; // positive if output, negative if input, 0 if no direction
	if(token[strlen(token)-1] == '>'){
		result.direction = 1;
		int parsed = sscanf(token, "%d", &result.fd);
		if(parsed != 1 && strlen(token)>1) // ls> 
			result.direction = 0; 
		else if(parsed < 1) //no numbers is scanned then it stdout ls  >
			result.fd = STDOUT_FILENO;
	}
	else if(token[strlen(token)-1] == '<'){
		result.direction = -1;
		int parsed = sscanf(token, "%d", &result.fd);
		if(parsed != 1 && strlen(token)>1)
			result.direction = 0;
		else if(parsed < 1)   //abc< < 
			result.fd = STDIN_FILENO;
	}
	return result;
}

RedirectionInfo BuildRedirection (Arraylist* tokenizeWithSpace_Quote){
	/*typedef struct RedirectionInfo{*/
	//int* in_file_arrayList;		/* store all in redirection to a file, size 0 if no file redirection */
	//Process* in_proc;			/* standard in process, NULL if no process redirection */
	//int in_fd;					/* used if redirect from a process, initialize to -1 */
	//int* out_file_arrayList;	/* store all out redirection to a file, size 0 if no file redirection */
	//Process* out_proc;			/* standard out process, NULL if no process redirection */
	//int out_fd;					/* used if redirect to a process, initialize to -1 */
//} RedirectionInfo;
	RedirectionInfo tokenWithSpace_Quote={
		Arraylist* in_file_array;
		
	
	}

}