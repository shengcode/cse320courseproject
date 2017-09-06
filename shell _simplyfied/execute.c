#include<stdlio.h>


#include "buildProcess.h"


void  execute_process(Job job,char**envp){
	//set up the pipes
	Process* process_cursor=job.first_process;
	while(process_cursor->next){
		int pipe_pair[2]; //pipe_pair[0] read end, pipe_pair[1] write end
		pipe(pipe_pair);
		process_cursor->fd_out=pipe_pair[1];
		process_cursor->next->fd_int=pipe_pair[0];	
		process_cursor=process_cursor->next;
	}
	process_cursor=job.first_process;
	while(process_cursor){
		if (execute_buildin(process_cursor))
	
	}

}


int execute_buildin(Process* process_cursor){
	//char* path=process_cursor->path; //cd exit export 
	if(strcmp(process_cursor->path,"cd")==0){
		if(chdir(process_cursor->argv[1])<0){
			printf("change directory failed\n");
		}
	}
	else if(strcmp(process_cursor->path,"exit")==0){
		exit(0);
	}
	else if (strcmp(process_cursor->path,"export")==0){
		
	}
	else{
		return -1;
	}
}


void isExport(char* export_argument,char**envp){
	char*new_key=malloc(strlen(export_argument)+1);
	char*new_value=malloc(strlen(export_argument)+1);
	extract_value_key(new_key,new_value,export_argument);
	
	
	
}

void extract_value_key(char*key, char*value,char*argument){
	int i;
	while(argument[i]!='=' && argument[i]!='\0'){
		key[i]=argument[i];
		i++;
	}
	strcpy(value,argument+i+1);
}