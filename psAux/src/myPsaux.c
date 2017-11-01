#include "../lib/myPsaux.h"

//***********************************************************
//************************ COMMANDE *************************
//***********************************************************
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

//***********************************************************
//************************* STATE ***************************
//***********************************************************
void afficher_state(char * path){
    printf("--State : ");

    char c;

    int idfichier = openFile(strcat(path,STAT));

    read_file_nbMot(idfichier,STATE_POSITION);


    read(idfichier,&c,sizeof(char));

    printf("%c\n",c);

    close(idfichier);
}

//***********************************************************
//************************* USER ****************************
//***********************************************************
int recup_uid(char * path){
    char * uid = NULL;
    int nb_mot = 0;
    int parcour_ligne = 0;
    FILE * fp = myFopen(strcat(path,STATUS));
    char * string_to_read = malloc(sizeof(char)*256);

    while(fgets(string_to_read, 256, fp)){
        if(strstr(string_to_read,UID))break;
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

//***********************************************************
//************************** TTY ****************************
//***********************************************************
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
        if(lecture->d_type == DT_CHR && strstr(lecture->d_name,TTY)){
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

//***********************************************************
//********************** START TIME  ************************
//***********************************************************

void afficher_start_time(char * path){
    printf("--Start time : ");
    
    struct stat buf_stat;
    stat(path, &buf_stat);

    time_t time = birthtime(buf_stat);
    struct tm * hours = localtime(&time);
    printf("%d:%d \n",hours->tm_hour,hours->tm_min);
}

//***********************************************************
//************************* RSS *****************************
//***********************************************************
int recup_rss(char * path){
    char * rss = get_value_by_key(path,"VmRSS",2,STATUS);
    return strcmp(rss,"")== 0 ? 0 : atoi(rss);
}

void afficher_rss(char * path){
    printf("--RSS : ");
 

    int rss = recup_rss(path);
    
    printf("%d \n",rss);
}

//***********************************************************
//************************* VSZ *****************************
//***********************************************************
int recup_vsz(char * path){
    char * vsz = get_value_by_key(path,"VmSize",2,STATUS);
    return strcmp(vsz,"")==0 ? 0 : atoi(vsz);
}

void afficher_vsz(char * path){
    printf("--VSZ : ");
 
    int vsz = recup_vsz(path);
    
    printf("%d \n",vsz);
}

//***********************************************************
//************************* TIME ****************************
//***********************************************************
int recup_time( int idfichier ){
    char c;
    char * time = "";

    while (read(idfichier,&c,sizeof(char)) != 0){ 
        if(c == ' ') break;
        time = concat_charactere(time,c);
    }

    return atoi(time);
}

void afficher_time(char * path){
    printf("--TIME : ");

    float utime,stime,cutime,cstime;

    int idfichier = openFile(strcat(path,STAT));

    read_file_nbMot(idfichier,TIME_POSITION);

    utime = recup_time(idfichier);
    stime = recup_time(idfichier);
    cutime = recup_time(idfichier);
    cstime = recup_time(idfichier);

    close(idfichier);

    //TO DO VERIF TIME
    long int time_target = cstime / sysconf(_SC_CLK_TCK) + cutime / sysconf(_SC_CLK_TCK);

    int minute , seconde ;
    minute = time_target / ONE_MINUTE;
    seconde = time_target - (minute * ONE_MINUTE);

    printf("%02d:%02d\n",minute,seconde);
}

//***********************************************************
//****************** MEMORY POURCENTAGE *********************
//***********************************************************
void afficher_memory_pourcentage(char * path){
    printf("--%%MEM : ");

    char * proc  = malloc((strlen(PROC)+1)*sizeof(char));
    char * meminfo  = malloc((strlen(MEM_INFO)+1)*sizeof(char));
    
    strcpy(proc,PROC);
    strcpy(meminfo,MEM_INFO);

    char * memory_total = get_value_by_key(proc,"MemTotal",2,meminfo);

    char * rss = get_value_by_key(path,"VmRSS",2,STATUS);

    float pourcentage = ( (atoi(rss) * 1.0) / atoi(memory_total) ) * 100;

    //TO DO : ARRONDI A L'INFERRIEUR
    printf("%3.1f %% \n" , pourcentage);

    free(proc);
    free(meminfo);
}

//***********************************************************
//******************** CPU POURCENTAGE **********************
//***********************************************************
int get_cpu_time(){
    int time =0 , parcour_ligne = 0 , nb_mot = 0;
    char * proc  = malloc((strlen(PROC)+1)*sizeof(char));
    char * stat  = malloc((strlen(STAT_FILE)+1)*sizeof(char));
    
    strcpy(proc,PROC);
    strcpy(stat,STAT_FILE);

    char * ligne = get_ligne(proc,"cpu",stat);

    printf("%s\n",ligne );
/*
    while(parcour_ligne < strlen(ligne)){
        if( nb_mot!=0 && ligne[parcour_ligne] == ' ' &&  ligne[parcour_ligne-1] != ' '){
            nb_mot++;
            time += atoi(value);        
            value = "";
        }
        value = concat_charactere(value,ligne[parcour_ligne]);
        parcour_ligne++;
    }*/

    free(proc);
    free(stat);

    return 0;
}


void afficher_cpu_pourcentage(char * path){
    int time_cpu = get_cpu_time();
}



//***********************************************************
//********************** READ PROC  *************************
//***********************************************************
void detailsProcessus(char * processusPID){
    
    printf("-----------------------------------\n");
    char * processus_path = recupPath(processusPID);

    afficher_user(copy_path(processus_path));
    afficher_cmdLine(copy_path(processus_path));
    afficher_state(copy_path(processus_path));
    afficher_tty(copy_path(processus_path));
    afficher_start_time(copy_path(processus_path));
    afficher_rss(copy_path(processus_path));
    afficher_vsz(copy_path(processus_path));
    afficher_time(copy_path(processus_path));
    afficher_memory_pourcentage(copy_path(processus_path));
    afficher_cpu_pourcentage(copy_path(processus_path));
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
