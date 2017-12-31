#ifndef LAUNCHMANAGER
#define LAUNCHMANAGER
#include "stack.h"
//#include "launchManager.h"
#include "operator.h"
#include "localMemory.h"
#include "./modules/mypipe/mypipe.h"
#include "./modules/myjoker/myjoker.h"
#include "./modules/myjobs/myjobs.h"
#include "./modules/mycd/mycd.h"
#include "./modules/myredir/myredir.h"


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
#include <time.h>


#define ARRAY_SIZE_DEFAULT 10
#define MYLS "myls"
#define MYPS "myps"
#define MYJOB "myjobs"
#define MYFG "myfg"
#define MYSTATUS "status"
#define MYCD "cd"
#define MYLS_PATH "/myls"
#define MYPS_PATH "/myps"

extern char **environ;

void displayStack(void);
void addToStack(char*);
void endInitStack(void);
void launchCommands(void);
void displayListCommand(void);
char **getCommandsArray(char*);
void destroyCommandsArray(char**);
void unStack(void);
void replace(char **t);
void replace2(char **t, int ind);

//handler
void change_sigint(void);
void freeList(void);

#endif
