#include "../lib/toolspsAux.h"

void concat_charactere(char * str , char c){

    int len = strlen(str);
    realloc(str,sizeof(char)*(len+1));

    str[len] = c;
    str[len+1] = '\0';
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
    int len = strlen(path);

    char * path_cmdline = malloc(sizeof(char)*(len+1));

    strcpy(path_cmdline,path);

    path[len] = '\0';

    return path_cmdline;
}

void read_file_nbMot(int idfichier,int position){
    int nb_mot = 0;
    char c;
    while ( nb_mot < position && read(idfichier,&c,sizeof(char)) != 0){ 
        if(c ==' ')nb_mot++;
    }
}


char * get_value_by_key(char * path,char * key,int nb_mot_max,char * file){
    char * value = creationChaineVide();
    int nb_mot = 0;
    int parcour_ligne = 0;

    FILE * fp = myFopen(strcat(path,file));

    char * string_to_read = malloc(sizeof(char)*256);

    while(fgets(string_to_read, 256, fp)){
        if(strstr(string_to_read,key))break;
    }
    
    if(strstr(string_to_read,key)){
        while(parcour_ligne < strlen(string_to_read)){
            if(nb_mot > nb_mot_max) break;
            if( string_to_read[parcour_ligne] == ' ' &&  string_to_read[parcour_ligne-1] != ' '){nb_mot++;}
            else if(nb_mot >= 1){
                concat_charactere(value,string_to_read[parcour_ligne]);
            }
         parcour_ligne++;
        }
    }

    myFclose(fp);

    free(string_to_read);
    return value;
}

char * get_ligne(char * path,char * key,char * file){

    FILE * fp = myFopen(strcat(path,file));

    char * string_to_read = malloc(sizeof(char)*256);

    while(fgets(string_to_read, 256, fp)){
        if(strstr(string_to_read,key))break;
    }

    myFclose(fp);

    return string_to_read;
}

char * creationChaineVide(){
    char * str = malloc(sizeof(char));
    str[0]='\0';
    return str;
}
