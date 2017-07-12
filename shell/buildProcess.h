#ifndef BUILDPROCESS_H
#define BUILDPROCESS_H
#include <sys/types.h>
#include "array_list.h"
void InQuotationReplacement(char* commd);
int BuildProcess(char*commd, char**env);
void TokenizeCommandStr(char* command,Arraylist* tokenizeWithPipe);
void TokenizeProcessStr(char* processStr,Arraylist* tokenizeWithSpace_Quote);
typedef struct Process{
	struct Process *next;
	char **argv;
	pid_t pid;
	char completed;
	char stopped;
	int status;
}Process;

typedef struct FileRedirectPair{
	int process_fd;
	char* file;
} FileRedirectPair;

typedef struct RedirectionInfo{
	Arraylist* in_file_arrayList;		/* store all in redirection to a file, size 0 if no file redirection */
	Process* in_proc;			/* standard in process, NULL if no process redirection */
	int in_fd;					/* used if redirect from a process, initialize to -1 */
	Arraylist* out_file_arrayList;	/* store all out redirection to a file, size 0 if no file redirection */
	Process* out_proc;			/* standard out process, NULL if no process redirection */
	int out_fd;					/* used if redirect to a process, initialize to -1 */
} RedirectionInfo;
#endif