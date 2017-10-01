#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <dirent.h>

#define PROC "/proc/"
#define CMDLINE "/cmdline"
#define STAT "/stat"

#define STATE_POSITION 2

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

void afficher_cmdLine(char * path){
    printf("--Cmdline : ");

    char * chemin = strcat(path,CMDLINE);
    char c;
    int idfichier = openFile(chemin);
    while (read(idfichier,&c,sizeof(char)) != 0){ 
        printf("%c", c);
    }
    printf("\n");
    close(idfichier);
}

void afficher_state(char * path){
    printf("--State : ");

    char * chemin = strcat(path,STAT);
    char c;

    int nb_mot = 1;

    int idfichier = openFile(chemin);
    while (read(idfichier,&c,sizeof(char)) != 0 && nb_mot <= STATE_POSITION){ 
        nb_mot = c ==' ' ? nb_mot+1 : nb_mot;
    }

    printf("%c\n",c);
    close(idfichier);
}

void detailsProcessus(char * processusPID){
	
	printf("-----------------------------------\n");
	char * processus_path = recupPath(processusPID);
    afficher_cmdLine(recupPath(processusPID));
    afficher_state(processus_path);


	printf("-----------------------------------\n");
}

void readProc(){
	struct dirent *lecture;
    DIR *rep;
    char *ptr;
    rep = opendir(PROC);
    while ((lecture = readdir(rep))) {
    	if(lecture->d_type == 4 && strtol(lecture->d_name, &ptr, 10)!=0){
        	printf("Numero processus : %s\n", lecture->d_name);
        	detailsProcessus(lecture->d_name);
        }
    }
    closedir(rep);
}

int main(void){
	readProc();
}