#include<stdio.h>
#include <stdint.h>
void changeValue(int * byte_towrite);
void CodePointToUTF(unsigned int unicode,int* byte_towrite);
int main(int argc, char**argv){
	int a=0;
	char* b=(char*) &a;
	*b=0XAF;
	printf("%x\n",*b);
	printf("my a is now %d\n",a);
	
	
	char c[4]={0};
	c[0]=0xAF;
	printf("%x\n",(*c));
	
	
	uint8_t what=0xEF;
	unsigned int whattest = (unsigned int) (what<<8);
	printf("whattest is %x\n",whattest);
	
	unsigned int whattest2 = ((unsigned int) what)<<8;
	printf("my second whattest is %x\n",whattest2);
	
	int byte_towrite =0;
	changeValue(&byte_towrite);
	printf("now my value became %d\n",byte_towrite);
	
	CodePointToUTF(29556, &byte_towrite);
	unsigned int test= 29556;
	if (test<0xFFFF && test>0x800){
		printf("it is correct\n");
	}
	uint8_t read_buffer[4]={0};
	int i;
	for(i=0;i<4;i++)
	printf("read buffer is %d\n",(int) read_buffer[i]);
	
	return 0;
}

void changeValue(int * byte_towrite){
	*byte_towrite=3;
}

void CodePointToUTF(unsigned int unicode,int* byte_towrite){
	printf("my unicode is now %d\n",unicode);
	if(unicode>0x0 && unicode<0x7F){
			*byte_towrite=1;
			printf("should be in 0x0 and 0x7F\n");
	}
	else if(unicode>0x80 && unicode<0x7FF){
			*byte_towrite=2;
			printf("should be in 0x80 and 0x7ff\n");
		}
		else if(unicode>0x800 && unicode<0xFFFF){
			*byte_towrite=3;
			printf("shuld be in 0x800 and 0xfff\n");
		}
		else if(unicode>0x10000 && unicode<0x1FFFF){
			*byte_towrite=4;
			printf("should be in 0x10000 and 0x1ffff\n");
		}
		else{
			printf("it is not in any range\n");
		}
	}















//char test[3]={0};
	//test[0]=0xAF;
	//int i;
	//for(i=0;i<3;i++)
	//printf("my char is %0x\n",test[i]);
	