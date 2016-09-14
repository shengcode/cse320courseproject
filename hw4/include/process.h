#ifndef PROCESS_H
#define PROCESS_H

int process(char*command, char**env);
void quotationReplace(char*command);
void quotationReturn (char* command);
void getProcessToken(char*command, ArrayList*processToken);
#endif