#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int pid_fils;

void handler_sigint(int arg){
	
	
	
	char res = ' ';
	char *buf = malloc(sizeof(char)*30);
	printf("\n\nDo you really wish to quit ?\n yes=y\n no=n\n");
	signal(SIGINT, SIG_IGN);
	
	
	if(kill(pid_fils,SIGSTOP)){
			perror("kill stop error");
			exit(1);
	}
	
	scanf(" %s",buf);
	res=buf[0];
	free(buf);
	
	//printf("res: >%c<\n", res);
	switch(res){
		case 'y':
			if(kill(pid_fils,SIGKILL)){
				perror("kill error");
				exit(1);
			}
			return;
			
		break;
		case 'n':
			printf("continue ps... \n");
			if(kill(pid_fils,SIGCONT)){
				perror("kill continue error");
				exit(1);
			}
			signal(SIGINT, handler_sigint);
			return;
		break;
		default:
			if(kill(pid_fils,SIGCONT)){
				perror("kill continue error");
				exit(1);
			}
			signal(SIGINT, handler_sigint);
			return;
		break;
	} 
	
	return;
	
}


int main(){

	int pid;
	pid = fork();
	if(pid<0){
		perror("fork error");
		exit(1);
	}
	//fils
	if(pid==0){
		execl("./shell","shell",NULL);
	}
	if(pid>0){
		pid_fils = pid; 
		signal(SIGINT, handler_sigint);
		
		int ret=-1;
		while(ret!=0&&ret!=9){
			wait(&ret);	
		}
		//printf("processus fils terminé, val ret: %d\n",ret);
	}

	
	
	

}
