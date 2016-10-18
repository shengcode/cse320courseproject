#ifndef PROCESS_H
#define PROCESS_H

//typedef enum ProcessState{};


int process(char*command, char**env);
void quotationReplace(char*command);
void quotationReturn (char* command);
void getProcessToken(char*command, ArrayList*processToken);
ArrayList* getSubProcessToken(char*processStr);




#endif