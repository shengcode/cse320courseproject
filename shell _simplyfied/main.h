#ifndef MAIN_H
#define MAIN_H
#include<termios.h>
#include "array_list.h"
int main (int argc, char ** argv, char ** envp);
void print_terminal(struct termios* term);
#endif