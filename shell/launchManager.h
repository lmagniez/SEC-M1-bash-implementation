#ifndef LAUNCHMANAGER
#define LAUNCHMANAGER
#include "stack.h"
#include "launchManager.h"
#include "operator.h"
#include "./modules/mypipe/mypipe.h"
#include "./modules/myjoker/myjoker.h"
#include "./modules/myjobs/myjobs.h"
#include "./modules/mycd/mycd.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ARRAY_SIZE_DEFAULT 10
#define MYLS "myls"
#define MYPS "myps"
#define MYJOB "myjobs"
#define MYFG "myfg"
#define MYCD "cd"
#define MYLS_PATH "./modules-externes/myls/myls"
#define MYPS_PATH "./modules-externes/myps/myps"

extern char **environ;

void displayStack(void);
void addToStack(char*);
void endInitStack(void);
void launchCommands(void);
void displayListCommand(void);
char **getCommandsArray(char*);
void destroyCommandsArray(char**);
void unStack(void);

//handler
void change_sigint(void);
void freeList(void);

#endif
