#include <stdio.h>
#include <math.h>

int main()
{
   //char str[80];

  // sprintf(str, "Value of Pi = %f", M_PI);
  // puts(str);
   
	char read_in[100];
	char read_buffer[4]={0x00,0x10,0x0f,0x7};
	
	int i;
	for(i=0;i<4;i++){
	sprintf(read_in+i*2, "%02x",(int)read_buffer[i]);
	}
	printf("read in is now %s\n",read_in);
	
	
	
	
	
   return(0);
}
