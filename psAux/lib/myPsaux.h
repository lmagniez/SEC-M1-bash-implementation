#include "./toolspsAux.h"

#include <dirent.h>

#include <pwd.h>

#define PROC "/proc/"
#define CMDLINE "/cmdline"
#define COMM "/comm"
#define STAT "/stat"
#define STATUS "/status"

#define NAME_POSITION 1
#define STATE_POSITION 2
#define TTY_POSITION 6
#define START_TIME_POSITION 5
#define READ_CMD_LINE 15

char * recup_comm(char * path);

void afficher_cmdLine(char * path);

void afficher_state(char * path);

int recup_uid(char * path);

void afficher_user(char * path);

char * getTTY(char * path);

void afficher_tty(char * path);

void afficher_start_time(char * path);

void detailsProcessus(char * processusPID);

void readProc();

void start_psaux();


