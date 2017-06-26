#ifndef BUILDPROCESS_H
#define BUILDPROCESS_H
#include <sys/types.h>
#include "array_list.h"
void InQuotationReplacement(Arraylist * stdinInput);
typedef struct process{
	struct process *next;
	char **argv;
	pid_t pid;
	char completed;
	char stopped;
	int status;
}process;
#endif