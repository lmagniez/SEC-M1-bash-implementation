#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <wait.h>


#define MAX_ARG 10
#define MAX_LEN 30


int oldout, newout, tempout;

void my_pipe(char **argv1, char **argv2){

	int pid1=1, pid2=1;
	int tube1[2];
	int tube2[2];
	if(pipe(tube1)<0||pipe(tube2)<0){
		perror("pipe error");
		exit(1);
	}


	pid1 = fork();
	if(pid1<0){
		perror("fork error");
		exit(1);
	}

	

	//first son
	//launch first command and write for the second son
	if(pid1==0){
		close(tube2[0]);
		close(tube2[1]);
		close(tube1[0]);
		
		oldout=dup(1);
		
		close(1);
		newout=dup(tube1[1]);
		close(tube1[1]);
		
		execvp(argv1[0],argv1);
		
		printf(">>end of fils 1\n");
		
		exit(0);
	}
	
	if(pid1>0){
		wait(NULL);
		printf(">>create fils 2\n");
		pid2 = fork();
		if(pid2<0){
			perror("fork error");
			exit(1);
		}
	}
	
	// second son
	// get the first son's result
	// execute second command and write for the father
	if(pid2==0){
		
		close(tube2[0]);
		close(tube1[1]);
		
		
		char c;
		char *res = malloc(sizeof(char)*MAX_LEN);
		int nb_char = 0;
		int max_len = MAX_LEN;
		
		int oldin, newin;
		oldin = dup(0);
		close(0);
		newin = dup(tube1[0]);
		close(tube1[0]);
		
		int oldout,newout;
		oldout = dup(1);
		close(1);
		newout = dup(tube2[1]);
		close(tube2[1]);
		
		execvp(argv2[0],argv2);
		
		exit(1);
	}
	
	// father
	// get the second son result and print it
	if(pid1>0&&pid2>0){
		
		close(tube2[1]);
		close(tube1[0]);
		close(tube1[1]);
		
		int res;
		
		char c;
		while(read(tube2[0], &c, sizeof(c))){
			printf("%c", c);
		}
		
		close(tube2[0]);
	}
}



int main(int argc, char **argv){

	char ** argv_cmd1 = malloc(sizeof(char*)*MAX_ARG);
	char ** argv_cmd2 = malloc(sizeof(char*)*MAX_ARG);
	
	argv_cmd1[0] = "ls";
	argv_cmd1[1] = "-l";
	argv_cmd1[2] = NULL;
	argv_cmd2[0] = "wc";
	argv_cmd2[1] = "-l";
	argv_cmd2[2] = NULL;
	
	
	my_pipe(argv_cmd1, argv_cmd2);
	
	
	free(argv_cmd1);
	free(argv_cmd2);

}
