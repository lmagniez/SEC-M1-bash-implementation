#include "../lib/myPsaux.h"


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

void afficher_state(char * path){
    printf("--State : ");

    char c;

    int idfichier = openFile(strcat(path,STAT));

    read_file_nbMot(idfichier,STATE_POSITION);


    read(idfichier,&c,sizeof(char));

    printf("%c\n",c);

    close(idfichier);
}

int recup_uid(char * path){
    int nb_ligne = 0;
    int parcour_ligne = 0;
    char * uid = NULL;
    int nb_mot = 0;
    FILE * fp = myFopen(strcat(path,STATUS));
    char * string_to_read = malloc(sizeof(char)*256);

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

char * read_dev_pts(char * tty){
    char * path = malloc(sizeof(char)*strlen(DEV_PTS));

    struct dirent *lecture;
    struct stat * buf_stat =  malloc(sizeof(struct stat));

    DIR *rep = opendir(DEV_PTS);

    while ((lecture = readdir(rep))) {
        if(lecture->d_type == DT_CHR){
            strcpy(path,DEV_PTS);
            path = strcat(path,lecture->d_name);
            if(stat(path, buf_stat)==-1){
                perror("stats");
                free(buf_stat);
                free(path);
            }
            if((int)buf_stat->st_rdev == atoi(tty)){
                free(buf_stat);
                return path;
            }
        }  
    }
    closedir(rep);

    free(buf_stat);
    free(path);
    return "";
}

char * read_dev(char * tty){
    char * path = malloc(sizeof(char)*strlen(DEV));

    struct dirent *lecture;
    struct stat * buf_stat =  malloc(sizeof(struct stat));

    DIR *rep = opendir(DEV);

    while ((lecture = readdir(rep))) {
        if(lecture->d_type == DT_CHR && strstr(lecture->d_name,"tty")){
            strcpy(path,DEV); 
            path = strcat(path,lecture->d_name); 

            if(stat(path, buf_stat)==-1){
                perror("stats");
                free(buf_stat);
                free(path);
            }
            if((int)buf_stat->st_rdev == atoi(tty)){
                free(buf_stat);
                return path;
            }
        }
    }
    closedir(rep);
    return "";
}

char * getNameTty(char * tty){ //mettre dans h
    char * name = read_dev_pts(tty);
    return !strcmp(name,"") == 0 ? name : read_dev(tty);
} 


void afficher_tty(char * path){
     printf("--TTY : ");

     char * tty = getTTY(path);

     char * name = atoi(tty) == 0 ? "?" : getNameTty(tty)+5;

     printf("%s \n",name);
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

void start_psaux(){
    readProc();
}
