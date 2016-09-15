#include<stdio.h>
#include<string.h>
#include<stdint.h>
int main(){
  /*char hostname[100]=" ";
  printf("%s\n",hostname);
  *hostname = 'U','T','F','1','8','9';
  // should understand char array and string 
//"UTF189";
  printf("%s\n",hostname);
 char * didi */
/*
 char * source = "hello";
 //char * dest;
 char dest[10]="";
for(int i =0; i<10;i++){
 printf("%d\n",*(dest+i));
}
 strncpy(dest, (source+2),2);
 printf("%s\n",dest);

   int a = 0b110;
  printf("%d\n",a);


unsigned int A = 0x12;
unsigned int B = 0x34;
unsigned int C = 0x56;
unsigned int A_P = A<<16;
unsigned int B_P=B<<8;
//unsigned Z = ((A_P | B_P)|C);
unsigned Z= A_P+B_P+C;
// both worked 
printf("%x\n",Z);
//uint16_t CC= ((uint16_t) AA)<<8+ BB;
uint8_t BB=0x00;
//uint16_t CC=  (AA<<8)+ BB;
//printf("%x\n",CC);
*/
uint8_t AA=0xE9;
printf("%x\n", (AA>>16));

char D= 0x10;
printf("%x\n", (D<<16));

uint8_t m [3]={0xEB,0xBB,0xBF};
printf("%x\n", m[0]);




unsigned int od =0x6d5b;
unsigned int oc = 0x10000;
if(od<oc){
   printf("great!\n");
}
 unsigned int a =0XFF;
 unsigned int b = 0Xff;
printf("%x\n",a);
printf("%d\n",b);









}
