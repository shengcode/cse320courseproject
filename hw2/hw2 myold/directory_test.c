#include<stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char**argv){
     char* absolute_directory = malloc(1000); 
     getcwd(absolute_directory, 1000);
     printf("%s\n", absolute_directory);
    // find the index of the last occurance of the slash
     int index_of_slash=0;
     char* char_position= strrchr(absolute_directory,'/');
    if(char_position!=NULL)
    index_of_slash= char_position-absolute_directory;
     printf("index of the last occurance of slash is %d\n",index_of_slash);

  // get the parent directory
    char parent_directory[1000];
    printf("not initialized %s\n", parent_directory);

    strcpy(parent_directory, absolute_directory);
    printf("test %s\n",parent_directory);


    *(parent_directory+index_of_slash)='\0';
     printf("%s\n",  parent_directory);
   // go to the parent directory  
   char parent[15]="/home/shengchun";
    chdir(parent);


}
