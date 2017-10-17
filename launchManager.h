#ifndef LAUNCHMANAGER
#define LAUNCHMANAGER
#include <stdio.h>
#include <stdlib.h>

struct CommandStruct {
	char *cmd1, *cmd2;
	char *operator;
	char *transitionOperator;
	struct CommandStruct* nextCmd;
};

typedef struct CommandStruct CommandStruct;

void displayStack(void);
void addToStack(char*);
CommandStruct* createListCommand(void);
void launchCommands(CommandStruct commands);
void displayListCommand(CommandStruct *cmd);

#endif