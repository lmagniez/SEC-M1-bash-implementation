#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <dirent.h>


#define PROC "/proc/"

#define syserror(x) perror(errormsg[x]), exit(x)
#define ROUGE(m) "\033[01;31m"m"\033[0m"
#define VERT(m) "\033[01;32m"m"\033[0m"
#define BLUE(m) "\033[01;34m"m"\033[0m"
#define CYAN(m) "\033[01;36m"m"\033[0m"

extern char *errormsg[];

void concat_charactere(char * str , char c);

DIR * myOpenDir(char * path);
void myCloseDir(DIR * rep);

FILE * myFopen(char * file);
void myFclose(FILE * fp);

int openFile(char * chemin);
void closeFile(int idFile);

char * recupPath(char * processus);

char * copy_path(char * path);

void read_file_nbMot(int idfichier,int position);

char * get_value_by_key(char * path,char * key,int nb_mot_max,char * file);

char * get_ligne(char * path,char * key,char * file);

char * creationChaineVide();
