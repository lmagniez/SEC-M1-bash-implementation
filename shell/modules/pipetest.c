#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <wait.h>


#define MAX_ARG 10
#define MAX_LEN 30


int oldout, newout, tempout;
/*int pipe1[2];
int pipe2[2];*/
int new_tube[2];
int old_tube[2];
int piperes[2];


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

void exec_cmd(char **cmd, int in, int at, int err){
	printf("EXEC CMD %s %d %d\n", cmd[0], in, at);
	printf("new: %d %d old: %d %d res: %d %d \n", 
		new_tube[0], new_tube[1], old_tube[0], old_tube[1], piperes[0], piperes[1]); 
	
	
	if(new_tube[1]&&at!=new_tube[1]){
		printf("close new_tube[1] %d \n", new_tube[1]);
		close(new_tube[1]);
	}
	if(old_tube[1]&&at!=old_tube[1]){
		printf("close old_tube[1] %d \n", old_tube[1]);
		close(old_tube[1]);
	}
	if(piperes[1]&&at!=piperes[1]){
		printf("close piperes[1] %d \n", piperes[1]);
		close(piperes[1]);
	}
	
	if(new_tube[0]&&in!=new_tube[0]){
		printf("close new_tube[0] %d \n", new_tube[0]);
		close(new_tube[0]);
	}
	if(old_tube[0]&&in!=old_tube[0]){
		printf("close old_tube[0] %d \n", old_tube[0]);
		close(old_tube[0]);
	}
	if(piperes[0]&&in!=piperes[0]){
		printf("close piperes[0] %d \n", piperes[0]);
		close(piperes[0]);
	}
	
	//if not stdin, redirect
	if(in != 0){
		printf("redirect in!\n");
		close(0);
		dup(in);
		close(in);
	}
	
	//if not stdout, redirect
	if(at != 1){
		printf("redirect at!\n");
		close(1);
		dup(at);
		close(at);
	}
	
	execvp(cmd[0],cmd);
	
} 

int main(int argc, char **argv){

	pid_t pid;
	char ** argv_cmd1 = malloc(sizeof(char*)*MAX_ARG);
	char ** argv_cmd2 = malloc(sizeof(char*)*MAX_ARG);
	char ** argv_cmd3 = malloc(sizeof(char*)*MAX_ARG);
	char ** argv_cmd4 = malloc(sizeof(char*)*MAX_ARG);
	
	
	argv_cmd1[0] = "ls";
	argv_cmd1[1] = "-l";
	argv_cmd1[2] = NULL;
	argv_cmd2[0] = "grep";
	argv_cmd2[1] = "loick";
	argv_cmd2[2] = NULL;
	argv_cmd3[0] = "head";
	argv_cmd3[1] = "-n";
	argv_cmd3[2] = "2";
	argv_cmd3[3] = NULL;
	argv_cmd4[0] = "wc";
	argv_cmd4[1] = "-l";
	argv_cmd4[2] = NULL;
	
	int nb_commande=4;
	char *** liste = malloc(sizeof(char**)*nb_commande);
	liste[0]=argv_cmd1;
	liste[1]=argv_cmd2;
	liste[2]=argv_cmd3;
	liste[3]=argv_cmd4;
	
	
	if(pipe(piperes)<0){
		perror("pipe error");
		exit(1);
	}
	for(int i=0; i<nb_commande; i++){
		
		int in=0, at=piperes[1];
		if(i>0){
			old_tube[0] = new_tube[0];
			old_tube[1] = new_tube[1];
			in = old_tube[0];
		}
		if(i!=nb_commande-1){
			if(pipe(new_tube)<0){
				perror("pipe error");
				exit(1);
			}
			at = new_tube[1];
		}
		
		
		pid = fork();
		if(pid < 0){
			perror("fork error");
			exit(1);
		}
		if(!pid){
			exec_cmd(liste[i], in, at, 3);
			//exec_cmd(argv_cmd3, pipe2[0], pipe1[1], 3);
		}
		if(i>0){
			close(old_tube[0]);
			close(old_tube[1]);
		}
	}
	
	
	/*
	//my_pipe(argv_cmd1, argv_cmd2);
	
	
	if(pipe(piperes)<0){
		perror("pipe error");
		exit(1);
	}
	
	//first cmd
	if(pipe(new_tube)<0){
		perror("pipe error");
		exit(1);
	}
	pid = fork();
	if(pid < 0){
		perror("fork error");
		exit(1);
	}
	if(!pid){
		exec_cmd(argv_cmd1, 0, new_tube[1], 3);
	}
	
	//second cmd
	old_tube[0] = new_tube[0];
	old_tube[1] = new_tube[1];
	if(pipe(new_tube)<0){
		perror("pipe error");
		exit(1);
	}
	pid = fork();
	if(pid < 0){
		perror("fork error");
		exit(1);
	}
	if(!pid){
		//exec_cmd(argv_cmd2, old_tube[0], piperes[1], 3);
		exec_cmd(argv_cmd2, old_tube[0], new_tube[1], 3);
	}
	close(old_tube[0]);
	close(old_tube[1]);
	
	//third cmd
	old_tube[0] = new_tube[0];
	old_tube[1] = new_tube[1];
	if(pipe(new_tube)<0){
		perror("pipe error");
		exit(1);
	}
	pid = fork();
	if(pid < 0){
		perror("fork error");
		exit(1);
	}
	if(!pid){
		exec_cmd(argv_cmd3, old_tube[0], new_tube[1], 3);
		//exec_cmd(argv_cmd3, pipe2[0], pipe1[1], 3);
	}
	close(old_tube[0]);
	close(old_tube[1]);
	
	//fourth cmd
	old_tube[0] = new_tube[0];
	old_tube[1] = new_tube[1];
	if(pipe(new_tube)<0){
		perror("pipe error");
		exit(1);
	}
	pid = fork();
	if(pid < 0){
		perror("fork error");
		exit(1);
	}
	if(!pid){
		exec_cmd(argv_cmd4, old_tube[0], piperes[1], 3);
		//exec_cmd(argv_cmd3, pipe2[0], pipe1[1], 3);
	}
	close(old_tube[0]);
	close(old_tube[1]);
	*/
	
	
	//////////////////////////////////////////
	// father: close the unused descriptors //
	//////////////////////////////////////////
	close(new_tube[0]);
	close(new_tube[1]);
	
	close(piperes[1]);
	
	char c;
	while(read(piperes[0],&c,1)!=0)
		putchar(c);
	
	close(piperes[0]);
	//close(new_tube[0]);
	
	
	free(argv_cmd1);
	free(argv_cmd2);

}
