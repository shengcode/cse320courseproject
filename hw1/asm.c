#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<inttypes.h>

int main(int argc, char**argv){
	char buffer[1024];
	fgets(buffer,1024,stdin);
	int64_t value = strtoll(buffer,NULL,10);
	printf("You entered %" PRId64 "\n",value);
	return EXIT_SUCCESS;
}
