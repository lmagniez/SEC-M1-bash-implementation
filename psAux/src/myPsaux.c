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
    return comm;
}

void afficher_cmdLine(char * path){
    printf("--Cmdline : ");
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

    if(!cmdline){
        cmdline = recup_comm(path);
    }
    close(idfichier);

    printf("%-15.30s \n",cmdline);

    free(path_cmdline);
    free(cmdline);
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
    char * tty = creationChaineVide();

    int idfichier = openFile(strcat(path,STAT));

    read_file_nbMot(idfichier,TTY_POSITION);

    while (read(idfichier,&c,sizeof(char)) != 0){ 
        if(c == ' ') break;
        concat_charactere(tty,c);
    }

    close(idfichier);
    return tty;
}

char * read_dev_pts(char * tty){
    char * path = malloc(sizeof(char)*strlen(DEV_PTS));
    char * vide = creationChaineVide();

    struct dirent *lecture;
    struct stat * buf_stat =  malloc(sizeof(struct stat));

    DIR *rep = opendir(DEV_PTS);

    while ((lecture = readdir(rep))) {
        if(lecture->d_type == DT_CHR){
            strcpy(path,DEV_PTS);
            path = strcat(path,lecture->d_name);
            if(stat(path, buf_stat)==-1){
                perror("stats");
                free(tty);
                free(buf_stat);
                free(path);
                exit(0);
            }
            if((int)buf_stat->st_rdev == atoi(tty)){
                free(tty);
                free(buf_stat);
                return path;
            }
        }  
    }
    closedir(rep);
    free(tty);
    free(buf_stat);
    return vide;
}

char * read_dev(char * tty){
    char * path = malloc(sizeof(char)*strlen(DEV));
    char * vide = creationChaineVide();

    struct dirent *lecture;
    struct stat * buf_stat =  malloc(sizeof(struct stat));

    DIR *rep = opendir(DEV);

    while ((lecture = readdir(rep))) {
        if(lecture->d_type == DT_CHR && strstr(lecture->d_name,TTY)){
            strcpy(path,DEV); 
            path = strcat(path,lecture->d_name); 

            if(stat(path, buf_stat)==-1){
                perror("stats");
                free(tty);
                free(buf_stat);
                free(path);
                exit(0);
            }
            if((int)buf_stat->st_rdev == atoi(tty)){
                free(tty);
                free(buf_stat);
                return path;
            }
        }
    }

    free(tty);
    free(buf_stat);
    closedir(rep);
    return vide;
}

char * getNameTty(char * tty){
    char * name = read_dev_pts(tty);
    if(!name[0] == '\0'){
        return name;
    }
    name = read_dev(tty);
    return name;
} 

void afficher_tty(char * path){
    printf("--TTY : ");

    char * tty = getTTY(path);

    if(atoi(tty) == 0){
        tty[0] = '?';
    }else{
        tty = getNameTty(tty)+5;
    }
    printf("%s \n",tty);
}

//***********************************************************
//********************** START TIME  ************************
//***********************************************************

void afficher_start_time(char * path){
    printf("--Start time: ");
    
    struct stat buf_stat;
    stat(path, &buf_stat);

    time_t time = birthtime(buf_stat);
    struct tm * hours = localtime(&time);
    printf("%02d:%02d \n",hours->tm_hour,hours->tm_min);
}

//***********************************************************
//************************* RSS *****************************
//***********************************************************
char * recup_rss(char * path){
    char * rss = get_value_by_key(path,"VmRSS",2,STATUS);
    return rss;
}

void afficher_rss(char * path){
    printf("--RSS : ");
    char * rss = recup_rss(path);
    printf("%d \n",atoi(rss));
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
    printf("--VSZ : ");
    char * vsz = recup_vsz(path);
    printf("%d \n",atoi(vsz));
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

void afficher_time(char * path){
    printf("--TIME :");

    float utime,stime,cutime,cstime;

    int idfichier = openFile(strcat(path,STAT));

    read_file_nbMot(idfichier,TIME_POSITION);

    utime = recup_time(idfichier);
    stime = recup_time(idfichier);
    cutime = recup_time(idfichier);
    cstime = recup_time(idfichier);
    
    close(idfichier);

    //TO DO VERIF TIME
    long int time_target = (cstime*1.0) / sysconf(_SC_CLK_TCK) + (cutime*1.0) / sysconf(_SC_CLK_TCK);

    int minute , seconde ;
    minute = time_target / ONE_MINUTE;
    seconde = time_target - (minute * ONE_MINUTE);

    printf(" %02d:%02d\n",minute,seconde);
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
        if(ligne[parcour_ligne] == ' '){
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
    printf("--%%CPU : ");
    int time_cpu = get_cpu_time();
    char * vsz = get_value_by_key(path,"VmSize",2,STATUS);
    float pourcentage = ( (atoi(vsz) * 1.0) / time_cpu ) * 100;

    printf("%3.1f %% \n" , pourcentage);

    free(vsz);
}

//***********************************************************
//********************** READ PROC  *************************
//***********************************************************
void detailsProcessus(char * processusPID){
    char * processus_path = recupPath(processusPID);

    char * copy;
    
    printf("-----------------------------------\n");
    copy = copy_path(processus_path);
    afficher_user(copy);
    free(copy);

    copy = copy_path(processus_path);
    afficher_cmdLine(copy);
    free(copy);
    
    copy = copy_path(processus_path);
    afficher_state(copy);
    free(copy);

    //MANQUE FREE
    copy = copy_path(processus_path);
    afficher_tty(copy);
    free(copy);

    copy = copy_path(processus_path);
    afficher_start_time(copy);
    free(copy);

    copy = copy_path(processus_path);
    afficher_rss(copy);
    free(copy);

    copy = copy_path(processus_path);
    afficher_vsz(copy);
    free(copy);

    copy = copy_path(processus_path);
    afficher_time(copy);
    free(copy);

    copy = copy_path(processus_path);
    afficher_memory_pourcentage(copy);
    free(copy);

    copy = copy_path(processus_path);
    afficher_cpu_pourcentage(copy);
    free(copy);
    
    printf("-----------------------------------\n");
    free(processus_path);
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
