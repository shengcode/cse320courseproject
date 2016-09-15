#include<stdlib.h>
#include<stdio.h>
#include<getopt.h>

void print_usage(){ 
    printf("Usage: temp-c<temp> | temp -f <temp>\n");
    exit(2);
}

int main(int argc, char ** argv){
   if(argc<2){
     print_usage();     
}
   int option;
   int cflag=0;
   int fflag=0;

   while((option=getopt(argc,argv,"cf"))!=-1){
      switch(option){
       case 'c':
          if(fflag){
          printf("Only one option allowed\n");
           exit(1);
       }
          else {
          cflag++;
          //fflag++;
            }
          printf("You want centigrade\n");
          break;
       case 'f':
           if(cflag){
             printf("only one option allowed\n");
             exit(1);
         }
           else{
              // cflag++;
               fflag++;
            }
           printf("You want farenheit\n");
           break;
       default:
            printf("Error message!");
       }
   }
}
