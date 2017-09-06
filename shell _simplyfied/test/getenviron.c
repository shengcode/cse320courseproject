#include<stdio.h>
#include<stdlib.h>
int main(int argc, char**argv ,char** environ){
	int i;
	printf("Command-line arguments:\n");
	for(i=0;environ[i]!=NULL;i++){
		printf("the %d the environment argument is %s\n",i,environ[i]);
	}
	char** test=malloc(sizeof(char*)*2);
	test[0]="abcdef";
	test[1]="jklm";
	
	environ=test;
	
	printf("new environ:\n");
	for(i=0;environ[i]!=NULL;i++){
		printf("the %d the environment argument is %s\n",i,environ[i]);
	}
	free(test);
}