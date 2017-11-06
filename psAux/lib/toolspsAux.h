#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>

#define PROC "/proc/"

void concat_charactere(char * str , char c);

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