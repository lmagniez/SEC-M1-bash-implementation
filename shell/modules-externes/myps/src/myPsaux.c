#include "../lib/myPsaux.h"

//***********************************************************
//************************ COMMANDE *************************
//***********************************************************
char * recup_comm(char * path){
    char * comm = creationChaineVide();
    char c;

    path = strcat(path,COMM);
    int idfichier = openFile(path);

    concat_charactere(comm,'[');
    
    while (read(idfichier,&c,sizeof(char)) != 0){ 
        if(c != '\n')
            concat_charactere(comm,c);
    }

    concat_charactere(comm,']');
    closeFile(idfichier);
    return comm;
}

void afficher_cmdLine(char * path){
    int nb_char = 0;
    char c;
    
    char * cmdline = creationChaineVide();
    char * path_cmdline = copy_path(path);
    path_cmdline = strcat(path_cmdline,CMDLINE);

    int idfichier = openFile(path_cmdline);
    
    while (read(idfichier,&c,sizeof(char)) != 0){ 
        if(nb_char < READ_CMD_LINE){
            concat_charactere(cmdline,c);
        }
        else
            break;
        nb_char++;
    }

    if(*cmdline == '\0'){
        cmdline = recup_comm(path);
    }

    closeFile(idfichier);

    printf("%-15.15s\t",cmdline);

    free(path_cmdline);
    free(cmdline);
}

//***********************************************************
//************************* STATE ***************************
//***********************************************************
void priority(char * path){
    char c ;
    char * priority = creationChaineVide();

    int idfichier = openFile(strcat(path,STAT));

    read_file_nbMot(idfichier,PRIORITY_POSITION);
    while (read(idfichier,&c,sizeof(char)) != 0){ 
        if(c == ' ') break;
        concat_charactere(priority,c);
    }

    closeFile(idfichier);

    if(strstr(priority,"-")){
        printf("<");
    }else if (atoi(priority) > 0){
        printf("N");
    }
    free(priority);
}

void pagesLockedMemory(char * path){
    char * number = get_value_by_key(path,"VmLck",2,STATUS);
    if( !(number[0] == '\0') && atoi(number) > 0)
            printf("L");
    free(number);
}

void sessionsLeader(char * path ,int processus){
    char * sid = get_value_by_key(path,"NSsid",2,STATUS);
    if(atoi(sid) == processus)printf("s");
    free(sid);
}

void multi_thread(char * path){
    int number = 0;
    struct dirent *lecture;
    char * directory_path = strcat(path,TASK);
    DIR *rep = myOpenDir(directory_path);
    
    while ((lecture = readdir(rep))) {
        if(lecture->d_type == DT_DIR && !strstr(lecture->d_name,".")){
            number++;
        }
    }

    myCloseDir(rep);
    if(number>1)printf("l");
}

void foreground(char * path,int processus){
    char c ;
    char * terminal_foreground = creationChaineVide();

    int idfichier = openFile(strcat(path,STAT));

    read_file_nbMot(idfichier,TERMINAL_FOREGROUND_POSITION);

    while (read(idfichier,&c,sizeof(char)) != 0){ 
        if(c == ' ') break;
        concat_charactere(terminal_foreground,c);
    }

    closeFile(idfichier);

    if(atoi(terminal_foreground) == processus)printf("+");
    free(terminal_foreground);
}

void afficher_state(char * path,int processusID){
    char c;

    char * copy = copy_path(path);
        int idfichier = openFile(strcat(copy,STAT));
    free(copy);

    read_file_nbMot(idfichier,STATE_POSITION);

    read(idfichier,&c,sizeof(char));

    printf("%c",c);
    copy = copy_path(path);
        priority(copy);
    free(copy);

    copy = copy_path(path);
        pagesLockedMemory(copy);
    free(copy);

    copy = copy_path(path);  
        sessionsLeader(copy,processusID);
    free(copy);

    copy = copy_path(path);
        multi_thread(copy);
    free(copy);

    copy = copy_path(path);
        foreground(copy,processusID);
    free(copy);

    printf("\t");
    closeFile(idfichier);
}

//***********************************************************
//************************* USER ****************************
//***********************************************************
int recup_uid(char * path){
    char * uid = creationChaineVide();
    int nb_mot = 0, parcour_ligne = 0 , uidValue = 0;

    FILE * fp = myFopen(strcat(path,STATUS));
    char * string_to_read = malloc(sizeof(char)*256);

    while(fgets(string_to_read, 256, fp)){
        if(strstr(string_to_read,UID))break;
    }

    while(parcour_ligne < strlen(string_to_read)){
        if(nb_mot >= 2) break;
        if(string_to_read[parcour_ligne] == '\t'){nb_mot++;}
        else if(nb_mot == 1){
            concat_charactere(uid,string_to_read[parcour_ligne]);
        }
         parcour_ligne++;
    }

    free(string_to_read);

    uidValue = atoi(uid);
    
    free(uid);
    myFclose(fp);

    return uidValue;
}

void afficher_user(char * path){
    struct passwd * pwd;
    pwd = getpwuid(recup_uid(path));
    if ( pwd != NULL)
        printf("%-8.8s", pwd->pw_name);
}

//***********************************************************
//************************** TTY ****************************
//***********************************************************
char * getTTY(char * path){
    char c ;
    char * tty = creationChaineVide();

    int idfichier = openFile(strcat(path,STAT));

    read_file_nbMot(idfichier,TTY_POSITION);

    while (read(idfichier,&c,sizeof(char)) != 0){ 
        if(c == ' ') break;
        concat_charactere(tty,c);
    }

    closeFile(idfichier);
    return tty;
}

char * read_dev_pts(char * tty){
    char * path = malloc(sizeof(char)*strlen(DEV_PTS));
    struct stat * buf_stat =  malloc(sizeof(struct stat));
    char * vide = creationChaineVide();

    struct dirent *lecture;

    DIR *rep = myOpenDir(DEV_PTS);

    while ((lecture = readdir(rep))) {
        if(lecture->d_type == DT_CHR){
            strcpy(path,DEV_PTS);
            path = strcat(path,lecture->d_name);
            if(stat(path, buf_stat)==-1){
                perror("stats");
                free(buf_stat);
                free(path);
                free(vide);
                exit(0);
            }
            if((int)buf_stat->st_rdev == atoi(tty)){
                free(buf_stat);
                free(vide);
                return path;
            }
        }  
    }

    myCloseDir(rep);
    free(buf_stat);
    free(path);
    return vide;
}

char * read_dev(char * tty){
    char * vide = creationChaineVide();
    char * path = malloc(sizeof(char)*strlen(DEV));
    struct dirent *lecture;
    struct stat * buf_stat =  malloc(sizeof(struct stat));

    DIR *rep = myOpenDir(DEV);

    while ((lecture = readdir(rep))) {
        if(lecture->d_type == DT_CHR && strstr(lecture->d_name,TTY)){
            strcpy(path,DEV); 
            path = strcat(path,lecture->d_name); 
            if(stat(path, buf_stat)==-1){
                perror("stats");
                free(buf_stat);
                free(path);
                free(vide);
                exit(0);
            }
            if((int)buf_stat->st_rdev == atoi(tty)){
                free(buf_stat);
                free(vide);
                return path;
            }
        }
    }

    myCloseDir(rep);
    free(buf_stat);
    free(path);
    return vide;
}

char * getNameTty(char * tty){
    char * name = read_dev_pts(tty);
    if(!name[0] == '\0'){
       return name;
    }
    name = read_dev(tty);
    free(tty);
    return name;
} 

void afficher_tty(char * path){

    char * tty = getTTY(path);
    if(atoi(tty) == 0){
        free(tty);
        tty = creationChaineVide();
        concat_charactere(tty,'?');
        printf("%s\t",tty );
    }else{
        tty = getNameTty(tty);
        printf("%s\t",tty+5);
    }
    free(tty);
}

//***********************************************************
//********************** START TIME  ************************
//***********************************************************

void afficher_start_time(char * path){
    struct stat buf_stat;
    stat(path, &buf_stat);

    time_t time = birthtime(buf_stat);
    struct tm * hours = localtime(&time);
    printf("%02d:%02d\t",hours->tm_hour,hours->tm_min);
}

//***********************************************************
//************************* RSS *****************************
//***********************************************************
char * recup_rss(char * path){
    char * rss = get_value_by_key(path,"VmRSS",2,STATUS);
    return rss;
}

void afficher_rss(char * path){
char * rss = recup_rss(path);
    printf("%d\t",atoi(rss));
    free(rss);
}

//***********************************************************
//************************* VSZ *****************************
//***********************************************************
char * recup_vsz(char * path){
    char * vsz = get_value_by_key(path,"VmSize",2,STATUS);
    return vsz;
}

void afficher_vsz(char * path){
    char * vsz = recup_vsz(path);
    printf("%d\t",atoi(vsz));
    free(vsz);
}

//***********************************************************
//************************* TIME ****************************
//***********************************************************
int recup_time( int idfichier ){
    char c;
    char * time = creationChaineVide();
    int timeValue = 0;

    while (read(idfichier,&c,sizeof(char)) != 0){ 
        if(c == ' ') break;
        concat_charactere(time,c);
    }

    timeValue = atoi(time);

    free(time);
    return timeValue;
}

long int recup_full_time(char * path){
    int idfichier = openFile(strcat(path,STAT));
    float utime,stime,cutime,cstime;
    read_file_nbMot(idfichier,TIME_POSITION);

    utime = recup_time(idfichier);
    stime = recup_time(idfichier);
    cutime = recup_time(idfichier);
    cstime = recup_time(idfichier);
    
    (void) utime;(void) stime;
	
    long int time_target = (cstime*1.0) / sysconf(_SC_CLK_TCK) + (cutime*1.0) / sysconf(_SC_CLK_TCK);
    close(idfichier);
    return time_target;
}

void afficher_time(char * path){
    long int time = recup_full_time(path);

    int minute , seconde ;
    minute = time / ONE_MINUTE;
    seconde = time - (minute * ONE_MINUTE);

    printf("%02d:%02d\t",minute,seconde);
}

//***********************************************************
//****************** MEMORY POURCENTAGE *********************
//***********************************************************
void afficher_memory_pourcentage(char * path){
    char * proc  = malloc((strlen(PROC)+1)*sizeof(char));
    char * meminfo  = malloc((strlen(MEM_INFO)+1)*sizeof(char));
    
    strcpy(proc,PROC);
    strcpy(meminfo,MEM_INFO);

    char * memory_total = get_value_by_key(proc,"MemTotal",2,meminfo);

    char * rss = get_value_by_key(path,"VmRSS",2,STATUS);

    float pourcentage = ( (atoi(rss) * 1.0) / atoi(memory_total) ) * 100;

    printf("%3.1f\t" , pourcentage);

    free(memory_total);
    free(rss);
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
    char * value = creationChaineVide();

    while(parcour_ligne < strlen(ligne)){
        if(ligne[parcour_ligne] == ' ' ){
            nb_mot++;
            time += atoi(value);     
            free(value);
            value = creationChaineVide();
        }
        concat_charactere(value,ligne[parcour_ligne]);
        parcour_ligne++;
    }

    free(ligne);
    free(proc);
    free(stat);
    free(value);
    return time;
}

void afficher_cpu_pourcentage(char * path){
    float time_cpu = get_cpu_time()*1.0 / sysconf(_SC_CLK_TCK);
    long int time = recup_full_time(path);
    float pourcentage = ( time / time_cpu ) * 100;

    printf("%3.1f\t" , pourcentage);
}

//***********************************************************
//********************** READ PROC  *************************
//***********************************************************
void detailsProcessus(char * processusID){
    char * processus_path = recupPath(processusID);

    char * copy;
    
    copy = copy_path(processus_path);
    afficher_user(copy);
    free(copy);

    printf("%s\t",processusID);
    
    copy = copy_path(processus_path);
    afficher_cpu_pourcentage(copy);
    free(copy);

    copy = copy_path(processus_path);
    afficher_memory_pourcentage(copy);
    free(copy);

    copy = copy_path(processus_path);
    afficher_vsz(copy);
    free(copy);

    copy = copy_path(processus_path);
    afficher_rss(copy);
    free(copy);

    copy = copy_path(processus_path);
    afficher_tty(copy);
    free(copy);

    copy = copy_path(processus_path);
    afficher_state(copy,atoi(processusID));
    free(copy);

    copy = copy_path(processus_path);
    afficher_start_time(copy);
    free(copy);

    copy = copy_path(processus_path);
    afficher_time(copy);
    free(copy);

    copy = copy_path(processus_path);
    afficher_cmdLine(copy);
    free(copy);

    free(processus_path);
    printf("\n");
}

void readProc(){
    struct dirent *lecture;
    DIR *rep = myOpenDir(PROC);
    char * ptr;
    while ((lecture = readdir(rep))) {
        if(lecture->d_type == DT_DIR && strtol(lecture->d_name, &ptr, 10)!=0){
            detailsProcessus(lecture->d_name);
        }
    }
    myCloseDir(rep);
}

void start_psaux(){
    printf("USER\tPID\t%%CPU\t%%MEM\tVSZ\tRSS\tTTY\tSTAT\tSTART\tTIME\tCOMMAND\n");
    readProc();
}
