#include "../lib/toolspsAux.h"

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
    fclose(fp);
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

void read_file_nbMot(int idfichier,int position){
    int nb_mot = 0;
    char c;
    while ( nb_mot < position && read(idfichier,&c,sizeof(char)) != 0){ 
        if(c ==' ')nb_mot++;
    }
}



