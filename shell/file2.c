#include<stdio.h>
#include "main.h"
#include "file2.h"
#include "file3.h"
int function2(){
	printf("you are calling function2\n");
	function3();
	return 1;
}