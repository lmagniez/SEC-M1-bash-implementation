#include "myjobs.h"

typedef struct {
	int pid;
	int is_fg;
	char *cmd;
} job;

job *list_jobs;
int nb_job;
int max_job = MAX_JOBS;

/*
 * JOBS
 */

void init_list_jobs(){
	list_jobs = malloc(sizeof(job)*max_job);
}

void set_fg(int id_job, int is_fg){
	list_jobs[id_job].is_fg = is_fg;
}

int is_fg(int id_job){
	return list_jobs[id_job].is_fg;
}


void ajout_job(int pid, char *cmd, int is_fg){
	//printf("ajout %d %s %d\n",pid, cmd, is_fg);
	
	if(nb_job == MAX_JOBS-1){
		max_job = max_job * 2;
		list_jobs = realloc(list_jobs,sizeof(job)*max_job);
	}
	list_jobs[nb_job].pid = pid;
	list_jobs[nb_job].is_fg = is_fg;
	list_jobs[nb_job].cmd = malloc(sizeof(char)*strlen(cmd)+1);
	strcpy(list_jobs[nb_job].cmd,cmd);
	list_jobs[nb_job].cmd[strlen(cmd)]='\0';
	nb_job++;
}

int get_id_job(int pid){
	//printf("get_id_job\n");
	for(int i=0; i<nb_job; i++){
		if(list_jobs[i].pid == pid)
			return i;
	}
	return -1;
}

int get_pid_from_id_job(int id_job){
	
	if(has_id(id_job)){
		return list_jobs[id_job].pid;
	}
	return -1;
}

void remove_job(int id_job){
	//printf("remove job [%d]\n",id_job);
	free(list_jobs[id_job].cmd);
	
	for(int i=id_job+1; i<nb_job; i++){
		list_jobs[i-1] = list_jobs[i];
	}
	nb_job--;
}

char * get_etat_job(int is_fg){
	if(is_fg == 1){
		return "Foreground";
	}
	else if(is_fg == 0){
		return "En cours d'execution";
	}
	return "Stoppé";
}

void afficher_job(){
	for(int i=0; i<nb_job; i++){
		job j = list_jobs[i];
		printf("[%d] %d %s %s\n", i, j.pid, get_etat_job(j.is_fg), j.cmd);
	}
}

int has_id(int id_job){
	return id_job>=0&&id_job<nb_job;
}

