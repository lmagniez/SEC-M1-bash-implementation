#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "../../psAux/lib/myPsaux.h"
#include "../../psAux/lib/toolspsAux.h"

int nbProcessus = 1;

int foreground(char * path,int processus){
	int retour = 1;
    char c ;
    char * terminal_foreground = creationChaineVide();

    int idfichier = openFile(strcat(path,STAT));

    read_file_nbMot(idfichier,TERMINAL_FOREGROUND_POSITION);

    while (read(idfichier,&c,sizeof(char)) != 0){ 
        if(c == ' ') break;
        concat_charactere(terminal_foreground,c);
    }

    closeFile(idfichier);

    if(atoi(terminal_foreground) == processus)retour = 0;
    free(terminal_foreground);
    return retour;
}

int terminal(char * path,int processus){
	int id = getppid(),idtty;

	char c ;
    char * tty = creationChaineVide();

    int idfichier = openFile(strcat(path,STAT));

    read_file_nbMot(idfichier,3);

    while (read(idfichier,&c,sizeof(char)) != 0){ 
        if(c == ' ') break;
        concat_charactere(tty,c);
    }
    closeFile(idfichier);
    idtty = atoi(tty);
    free(tty);
    return idtty == id;
}

char getstate(char * path,int processus){
	char c;

	int idfichier = openFile(strcat(path,STAT));

    read_file_nbMot(idfichier,STATE_POSITION);

    read(idfichier,&c,sizeof(char));

    return c;
}

char * getcommande(char * path,int processus){
	int nb_char = 0;
    char c;
    
    char * cmdline = creationChaineVide();

    path = strcat(path,CMDLINE);

    int idfichier = openFile(path);
    
    while (read(idfichier,&c,sizeof(char)) != 0){ 
        if(nb_char < READ_CMD_LINE){
            concat_charactere(cmdline,c);
        }
        else
            break;
        nb_char++;
    }


    closeFile(idfichier);
    return cmdline;
}

int verifProcessus(char * processusID){
	char * copy;
 	char * processus_path = recupPath(processusID);
 	int tty,arrierePlan,id = atoi(processusID);

	copy = copy_path(processus_path);
    arrierePlan = foreground(copy,id);
    free(copy);

    if(!arrierePlan){
    	free(processus_path);
    	return 0;
    }

    copy = copy_path(processus_path);
    tty = terminal(copy,id);
    free(copy);

    if(!tty){
    	free(processus_path);
    	return 0;
    }

    free(processus_path);
    return 1;
}

char * correspondanceState(char c){
	switch(c){
		case 'T':
			return "Stopped";
		break;
		default:
			return "Running";
		break;
	}
}

void affichage(char * processusID){
	int id = atoi(processusID);
	char state;
	char * copy;
 	char * processus_path = recupPath(processusID);
 	char * commande;

	copy = copy_path(processus_path);
    state = getstate(copy,id);
    free(copy);

    copy = copy_path(processus_path);
    commande = getcommande(copy,id);
    free(copy);

    printf("[%d]   %s\t %s\t %s\t \n",nbProcessus++,processusID,correspondanceState(state),commande);

    free(commande);
    free(processus_path);
}

void readProc(){
    struct dirent *lecture;
    DIR *rep = myOpenDir(PROC);
    char * ptr;
    while ((lecture = readdir(rep))) {
        if(lecture->d_type == DT_DIR && strtol(lecture->d_name, &ptr, 10)!=0){
            if(!verifProcessus(lecture->d_name))continue;
            affichage(lecture->d_name);
        }
    }
    myCloseDir(rep);
}

int main(){
	readProc();
}
