#include<stdio.h>
#include<stdint.h>
void readmore(FILE* filetoread);
int main(int argc, char**argv){
	FILE* file;
	if( (file=fopen("filepointer.txt","r"))==NULL){
		printf("failed to open the file\n");
	}
	int a=fseek(file, 2, SEEK_SET);
	printf("the value returned by fseek is %d\n",a);
	
	char ptr[100]={0};
	fread(ptr, 1, 5, file);
	printf("the staff I just read are %s\n",ptr);
   
	readmore(file);
	fclose(file);
	return 0;
}

void readmore(FILE* file){
	uint8_t ptr[100]={0};
	fread(ptr,1,5,file);
	printf("the staff I just read are %s\n",(char*)ptr);
	
	
}