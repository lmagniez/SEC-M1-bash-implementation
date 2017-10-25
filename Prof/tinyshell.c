#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

#define LEN_CMD 100
#define NB_CMD_MAX 10



//divide the command into a table of command (get rid of the ;)
char** divide_cmd(char *cmd){
	int crs = 0;	
	int start_cmd = 0;
	int nb_cmd = 0;
	int len_cmd = 0;
			
	char ** tab_cmd = malloc(sizeof(char*)*NB_CMD_MAX);
	while(cmd[crs]){
		if(cmd[crs] == ';'){
			if(crs-start_cmd != 0){
				len_cmd = crs - start_cmd; 
				tab_cmd[nb_cmd] = malloc(sizeof(char)*len_cmd+1);
				strncpy(tab_cmd[nb_cmd],&cmd[start_cmd],len_cmd);
				tab_cmd[nb_cmd++][len_cmd] = '\0'; //strncpy doesn't add the NULL terminator
			}
			start_cmd = crs+1;
		}
		crs++;
	}
	len_cmd = crs - start_cmd; 
	tab_cmd[nb_cmd] = malloc(sizeof(char)*len_cmd+1);
	strncpy(tab_cmd[nb_cmd],&cmd[start_cmd],len_cmd);
	tab_cmd[nb_cmd++][len_cmd] = '\0'; //strncpy doesn't add the NULL terminator

	/*
	int crs2=0;
	while(tab_cmd[crs2]){
		printf("cmd -> %s \n",tab_cmd[crs2++]);
	}*/

	return tab_cmd;
}

int exec_cmd(char *cmd){
	pid_t p = fork();
	int s;
	if(p==-1){
		perror("fork");
		return -1;
	}
	if(p==0){
		//divide_cmd(cmd);
		char *vcmd[] = {"/bin/sh","-c",cmd, NULL};
		execv("/bin/sh",vcmd);
		perror("execv");
		exit(127);
	}
	wait(&s);
	
	if(WIFEXITED(s)){
		return WEXITSTATUS(s);
	}
	return -1;
}

void tiny_shell(){

	int res_cmd;
	char* cmd=malloc(sizeof(char)*LEN_CMD);
	
	while(1){
		printf("> ");
		fgets(cmd,LEN_CMD,stdin);
		printf("%s\n",cmd); 
		res_cmd = exec_cmd(cmd);

		//search for the ; and && 
		printf("Return value: %d \n", res_cmd);
	}
	
	free(cmd);
}



int main(void){

	tiny_shell();

	exit(0);
}
