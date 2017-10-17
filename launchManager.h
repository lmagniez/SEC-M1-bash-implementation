#ifndef LAUNCHMANAGER
#define LAUNCHMANAGER
#include "stack.h"
#include "launchManager.h"
#include "operator.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void displayStack(void);
void addToStack(char*);
void endInitStack(void);
void launchCommands(void);
void displayListCommand(void);

#endif