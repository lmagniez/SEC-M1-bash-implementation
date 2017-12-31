#include "myjobs.h"
#define KEY_JOB 10005
#define KEY_NB_JOB 10002
#define KEY_MAX_JOB 10003

int key_job;
int key_nb_job;
int key_max_job;

typedef struct {
	int pid;
	int is_fg;
	//char *cmd;
	char cmd[MAX_JOB_CHAR];
} job;

job *list_jobs;
int nb_job;
int max_job = MAX_JOBS;

/*
 * JOBS
 */

key_t generate_cle(const char * path , int id){
	key_t key = ftok(path,id);
	if(key==-1){
		perror("ftok()");
		exit(1);
	}
	return key;
}

void init_list_jobs(){
	
	list_jobs = malloc(sizeof(job)*max_job);
	/*
	int number = 20;
	int mem_id;

	int * reference_zone_memoire;

	key_t key = generate_cle("/bin/ls",1);

	printf("CREATION MEM\n");
	mem_id = shmget(key,sizeof(int)*max_job,IPC_EXCL | IPC_CREAT | 0777);
	if(mem_id<0){
		mem_id = shmget(key,sizeof(int)*max_job,0777);
	}

	//attache zone mémoire
	printf("ATTACHE MEM\n");
	reference_zone_memoire = (int*) shmat(mem_id,0,0);
	if(reference_zone_memoire == NULL){
		perror("shmat()");
		exit(1);
	}



	printf("ECRITURE MEM\n");
	*reference_zone_memoire = (int)number;
	printf("ECRITURE MEM\n");
	*(reference_zone_memoire+sizeof(int)) = 21;
	



	printf("LECTURE MEM\n");
	number = *reference_zone_memoire;
	printf("----> %d\n",number);
	number = *(reference_zone_memoire+sizeof(int));
	printf("----> %d\n",number);




	//detache le segment
	printf("DETACHEMENT MEM\n");
	if(shmdt(reference_zone_memoire) == -1){
		perror("shmat()");
		exit(1);
	}

	if(shmctl(mem_id,IPC_RMID,0) == -1){
		perror("shmctl()");
		exit(1);
	}
	*/

/*	
	int number = 20;
	int mem_id;

	int * reference_zone_memoire;

	key_job = generate_cle("/bin/ls",1);

	printf("CREATION MEM %d\n", sizeof(job));
	mem_id = shmget(key_job,(sizeof(job))*max_job,IPC_EXCL | IPC_CREAT | 0777);
	if(mem_id<0){
		mem_id = shmget(key_job,(sizeof(job))*max_job,0777);
	}

	//attache zone mémoire
	printf("ATTACHE MEM\n");
	list_jobs = (job*) shmat(mem_id,0,0);
	if(list_jobs == NULL){
		perror("shmat()");
		exit(1);
	}
*/

	/*
	printf("ECRITURE MEM\n");
	*reference_zone_memoire = (job)number;
	printf("ECRITURE MEM\n");
	*(reference_zone_memoire+sizeof(job)) = 21;
	*/


	/*
	printf("LECTURE MEM\n");
	number = *reference_zone_memoire;
	printf("----> %d\n",number);
	number = *(reference_zone_memoire+sizeof(int));
	printf("----> %d\n",number);
	*/



	//detache le segment
	
	//list_jobs = malloc(sizeof(job)*max_job);
	
/*
	ajout_job(12, "blabla", 1);
	afficher_job();
*/	
	
	
	/*
	printf("DETACHEMENT MEM\n");
	if(shmdt(list_jobs) == -1){
		perror("shmat()");
		exit(1);
	}

	if(shmctl(mem_id,IPC_RMID,0) == -1){
		perror("shmctl()");
		exit(1);
	}*/
	
	
	
	
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
		list_jobs = realloc(list_jobs,(sizeof(job))*max_job);
	}
	list_jobs[nb_job].pid = pid;
	list_jobs[nb_job].is_fg = is_fg;
	//list_jobs[nb_job].cmd = malloc(sizeof(char)*strlen(cmd)+1);
	strcpy(list_jobs[nb_job].cmd,cmd);
	list_jobs[nb_job].cmd[strlen(cmd)]='\0';
	nb_job++;
}

/*
void ajout_job(int pid, char *cmd, int is_fg){
	printf("ajout %d %s %d\n",pid, cmd, is_fg);
	
	if(nb_job == MAX_JOBS-1){
		max_job = max_job * 2;
		//list_jobs = realloc(list_jobs,(sizeof(job)+MAX_JOB_CHAR)*max_job);
	}
	job * j = (list_jobs+sizeof(job)*nb_job);
	j->pid = pid;
	j->is_fg = is_fg;
	//list_jobs[nb_job].cmd = malloc(sizeof(char)*strlen(cmd)+1);
	strcpy(j->cmd,cmd);
	j->cmd[strlen(cmd)]='\0';
	nb_job++;
}
*/

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
	//free(list_jobs[id_job].cmd);
	
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

/*
void afficher_job(){
	printf("affiche\n");
	for(int i=0; i<nb_job; i++){
		job *j = (list_jobs+sizeof(job)*i);
		printf("[%d] %d %s %s\n", i, j->pid, get_etat_job(j->is_fg), j->cmd);
	}
}
*/

int has_id(int id_job){
	return id_job>=0&&id_job<nb_job;
}

