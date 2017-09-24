#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <dirent.h>

#define PROC "/proc/"

void detailsProcessus(char * processusPID){
	
	printf("-----------------------------------\n");
	DIR *rep;
	
	char *path;
    path=(char *)malloc((strlen(PROC)+strlen(processusPID))*sizeof(char));
	strcpy(path,PROC);
    strcpy(path,processusPID);

	rep = opendir(path);
	printf("-----------------------------------\n");
}

void readProc(){
	struct dirent *lecture;
    DIR *rep;
    char *ptr;
    rep = opendir(PROC);
    while ((lecture = readdir(rep))) {
    	if(lecture->d_type == 4 && strtol(lecture->d_name, &ptr, 10)!=0){
        	printf("%s\n", lecture->d_name);
        	detailsProcessus(lecture->d_name);
        }
    }
    closedir(rep);
}

int main(void){
	readProc();
}