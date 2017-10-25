#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <dirent.h>

#include <pwd.h>

#define DEV_PTS "/dev/pts"
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

char * concat_charactere(char * str , char c){

    int len = str == NULL ? 0 : strlen(str);

    str = str == NULL ? malloc(sizeof(char)*2) : str;

    char * newChaine = malloc(sizeof(char)*(len+1));

    strcpy(newChaine,str);

    newChaine[len] = c;
    newChaine[len+1] = '\0';

    return newChaine;
}

FILE * myFopen(char * file){
    FILE * fp = fopen(file,"r");

    if(fp == NULL){
        perror("Le fichier n'existe pas \n");
        exit(1);
    }

    return fp;
}


void myFclose(FILE * fp){
    int retour = fclose(fp);
}

int openFile(char * chemin){
    int idfichier=open(chemin,O_RDONLY);
    if(idfichier == -1){
        perror("Le fichier n'existe pas \n");
    }
    return idfichier;
}

void closeFile(int idFile){
    close(idFile);
}

char * recupPath(char * processus){
    char *path = (char *)malloc((strlen(PROC)+strlen(processus))*sizeof(char));
    strcpy(path,PROC);
    strcat(path,processus);
    return path;
}

char * copy_path(char * path){
    char * path_cmdline = malloc(sizeof(char)*strlen(path)+1);

    strcpy(path_cmdline,path);

    return path_cmdline;
}

char * recup_comm(char * path){
    char * comm = NULL;
    char c;

    int idfichier = openFile(strcat(path,COMM));

    comm = concat_charactere(comm,'[');

    while (read(idfichier,&c,sizeof(char)) != 0){ 
        if(c != '\n')
            comm = concat_charactere(comm,c);
    }

    comm = concat_charactere(comm,']');

    return comm;
}

void afficher_cmdLine(char * path){
    printf("--Cmdline : ");
    int nb_char = 0;
    char * cmdline = NULL;
    char c;
    
    char * path_cmdline = copy_path(path);

    int idfichier = openFile(strcat(path_cmdline,CMDLINE));
    
    while (read(idfichier,&c,sizeof(char)) != 0){ 
        if(nb_char < READ_CMD_LINE){
            cmdline = concat_charactere(cmdline,c);
        }
        else
            break;
        nb_char++;
    }

    if(!cmdline){
        cmdline = recup_comm(path);
    }
  
    printf("%-15.30s \n",cmdline);
    close(idfichier);
}


void read_file_nbMot(int idfichier,int position){
    int nb_mot = 0;
    char c;
    while ( nb_mot < position && read(idfichier,&c,sizeof(char)) != 0){ 
        if(c ==' ')nb_mot++;
    }
}



void afficher_state(char * path){
    printf("--State : ");

    char c;

    int nb_mot = 1;

    int idfichier = openFile(strcat(path,STAT));

    read_file_nbMot(idfichier,STATE_POSITION);


    read(idfichier,&c,sizeof(char));

    printf("%c\n",c);

    close(idfichier);
}

int recup_uid(char * path){

    FILE * fp = myFopen(strcat(path,STATUS));

    int nb_ligne = 0;
    int parcour_ligne = 0;
    char * string_to_read = malloc(sizeof(char)*256);
    char * uid = NULL;
    int nb_mot = 0;


    while(fgets(string_to_read, 256, fp) && nb_ligne < 9){
        nb_ligne++;
    }

    while(parcour_ligne < strlen(string_to_read)){
        if(nb_mot >= 2) break;
        if(string_to_read[parcour_ligne] == '\t'){nb_mot++;}
        else if(nb_mot == 1){
            uid = concat_charactere(uid,string_to_read[parcour_ligne]);
        }
         parcour_ligne++;
    }

    myFclose(fp);
    return atoi(uid);
}

void afficher_user(char * path){
    printf("--User : ");
 
    struct passwd * pwd;

    pwd = getpwuid(recup_uid(path));

    if ( pwd != NULL)
        printf(" %-8.8s \n", pwd->pw_name);
}


char * getTTY(char * path){
    char c ;
    char * tty = "";

    int idfichier = openFile(strcat(path,STAT));

    read_file_nbMot(idfichier,TTY_POSITION);

    while (read(idfichier,&c,sizeof(char)) != 0){ 
        if(c == ' ') break;
        tty = concat_charactere(tty,c);
    }

    close(idfichier);

    return tty;
}


char * nametyy(int tty){
	struct dirent *lecture;
	DIR *rep = opendir(DEV_PTS);
	char * ptr;
	struct stat * fileStat = malloc(sizeof(struct stat));

    printf("-------\n");
    return "";
}


void afficher_tty(char * path){
     printf("--TTY : ");

     char * tty = getTTY(path);
     int tty_id = atoi(tty);

     printf("%s ", tty);

     char * name = tty_id == 0 ? "?" : nametyy(tty_id);

     printf("----- %-8.8s \n",name );
}


void afficher_start_time(char * path){
    printf("--Start time : ");

   long unsigned int start_time;

    int idfichier = openFile(strcat(path,STAT));

    read_file_nbMot(idfichier,START_TIME_POSITION);


    read(idfichier,&start_time,sizeof(long unsigned int ));

    close(idfichier);

    printf(" %lu \n",start_time);
}

void detailsProcessus(char * processusPID){
    
    printf("-----------------------------------\n");
    char * processus_path = recupPath(processusPID);

    afficher_user(copy_path(processus_path));
    afficher_cmdLine(copy_path(processus_path));
    afficher_state(copy_path(processus_path));
    afficher_tty(copy_path(processus_path));
    afficher_start_time(copy_path(processus_path));

    printf("-----------------------------------\n");
}

void readProc(){
    struct dirent *lecture;
    DIR *rep = opendir(PROC);
    char * ptr;
    while ((lecture = readdir(rep))) {
        if(lecture->d_type == DT_DIR && strtol(lecture->d_name, &ptr, 10)!=0){
            printf("Numero processus : %s\n", lecture->d_name);
            detailsProcessus(lecture->d_name);
        }
    }
    closedir(rep);
}

int main(void){
    readProc();
}
