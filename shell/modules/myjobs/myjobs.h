#ifndef MYJOBS
#define MYJOBS

#define MAX_JOBS 30
#define MAX_JOB_CHAR 50

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>


void init_list_jobs();
void ajout_job(int pid, char *cmd, int is_running);
void remove_job(int id_job);
char * get_etat_job(int is_running);
void afficher_job();
int get_id_job(int pid);
void put_to_sleep_ps();
int has_id(int id_job);
int get_pid_from_id_job(int id_job);
void set_fg(int id_job, int is_fg);
int is_fg(int id_job);
#endif
