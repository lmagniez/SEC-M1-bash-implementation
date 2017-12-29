#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "mycd.h"

#define HOME "HOME"

/*
#define DELIMITEUR '/'
char ** recup_liste_fichier(char * chemin,int * size){
	char ** listeF = malloc(sizeof(char));
	char * partie = malloc(sizeof(char));
	int len_listeF = 0,len_partie = 0;
	
	for(int i = 0;i<=strlen(chemin);i++){	
		if(chemin[i]==DELIMITEUR || i==strlen(chemin) ){
			realloc(listeF,len_listeF+1);
			listeF[len_listeF] = malloc(strlen(partie)+1);
			strcpy(listeF[len_listeF],partie);
			len_listeF++;
			*size += 1;
			len_partie = 0;
		}else{
			partie[len_partie] = chemin[i];
			realloc(partie,len_partie+1);
			len_partie++;	
		}
	}
	return listeF;
}
void cd(char * chemin){
	int size = 0;
	char ** listeF = recup_liste_fichier(chemin,&size);
	for(int i=0;i<size;i++){
		deplacement_dossier(listeF[i]);
	}
	free(listeF);
}*/

char * verification_home(char * chemin){
	return *chemin == '\0' ? getenv(HOME) : chemin;
}


int cd(char * chemin){
	int res = chdir(verification_home(chemin));
	if(res == -1){
		perror("Probleme");
		return -1;
	}
	return 0;
}

void ls(){
	struct dirent *lecture;
    	DIR *rep = opendir(".");
   	while ((lecture = readdir(rep))) {
            printf("%s \n",lecture->d_name);
        }
    	closedir(rep);
}

/*
int main(int argc, char **argv){
	//cd("");
	cd(argv[1]);
	ls();
}*/
