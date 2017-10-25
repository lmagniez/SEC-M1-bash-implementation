#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>

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

char * concat_charactere(char * str , char c);

FILE * myFopen(char * file);

void myFclose(FILE * fp);

int openFile(char * chemin);

void closeFile(int idFile);

char * recupPath(char * processus);

char * copy_path(char * path);

void read_file_nbMot(int idfichier,int position);