#ifndef MY_JOKER
#define MY_JOKER
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define syserror(x) perror(errormsg[x]), exit(x)
#define ROUGE(m) "\033[01;31m"m"\033[0m"
#define VERT(m) "\033[01;32m"m"\033[0m"
#define BLUE(m) "\033[01;34m"m"\033[0m"
#define CYAN(m) "\033[01;36m"m"\033[0m"

#define MAX_ARG 10
#define MAX_LEN 30

#define MAX_PATH 10
#define MAX_FILE 10
#define MAX_DIR 10
#define RIGHT_LEN 10
#define MAX_FILENAME 100



void lire_doc(char * path);
void get_path_from_expression(char * expression, char **folder, char **elt, char **fin);
int elt_belong_to_expr(char *expr, char *elt);
int search_in_dir(char *folder, char *elt, char *fin, int search_folder, char ***res);
char ** get_elements(char *expr);
char ** replace_cmdarray(char** commandArray, int ind, char **add );
char *get_str_from_interval(char a, char b);
char* get_str_from_expr(char *expr);


#endif
