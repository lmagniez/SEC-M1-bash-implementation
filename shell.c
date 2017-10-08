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
#define COMM "/comm"
#define STAT "/stat"

#define STATE_POSITION 2
#define READ_CMD_LINE 15

char * concat_charactere(char * str , char c){

    int len = str == NULL ? 0 : strlen(str);

    str = str == NULL ? malloc(sizeof(char)) : str;

    char * newChaine = malloc(sizeof(char)*(len+1));

    strcpy(newChaine,str);
    newChaine[len] = c;

    return newChaine;
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

char * recup_comm(char * path){
    char * comm = malloc(sizeof(char));
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
    char * path_cmdline = malloc(sizeof(char)*strlen(path)+1);

    strcpy(path_cmdline,path);


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
  
    printf("%s \n",cmdline);
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