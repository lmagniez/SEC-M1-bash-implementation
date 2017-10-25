#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>

#define FAILEDEXEC 127
#define ERR -1
#define sizelgcmd 4096 //doit faire un alloc dynamique (/!\ fuite mémoire)
#define size 1024 // nb arg

#define syserr(x) perror(errormsg[x])
#define fatalsyserror(x) syserror(x), exit(x)
#define ROUGE(m) "\033[01;31m"m"\033[0m"
#define VERT(m) "\033[01;32m"m"\033[0m"

char *errormsg[] = {
	"No error", //0
	ROUGE("Impossible to fork process"), //1
	ROUGE("Exec failed") //2
};

int main(void){
	char lgcmd[sizelgcmd], *tabcmd[size], *s, **ps;
	pid_t p;
	int i, status;

	for(;;){
		putchar('>');
		fgets(lgcmd,sizelgcmd-1,stdin);
		for(s=lgcmd;isspace(*s);s++);//positionne au dernier espace
		for(i=0;*s;i++){
			tacmd[i]=s;//récupère juste l'adresse
			while(!isspace(*s))s++;
			*s++='\0';
			while(isspace(*s))s++;
		}
		if(i){
			tabcmd[i]=NULL;
			if((p=fork())==ERR) fatalsyserror(1);
			if(p){
				wait(&status);
				if(WIFEXITED(status)){
					if((status=WEXITSTATUS(status))!=FAILEDEXEC) {
						printf(VERT("exit status of ["));
						for(ps=tabcmd;*ps;ps++) printf("%s", *ps);
						printf(VERT("\b]=%d\n"),status);
					}
				}
				else puts(ROUGE("Abnormal exit"));
			}
			else {
				execvp(*tabcmd,tabcmd);
				syserror(2);
				exit(FAILEDEXEC);
			}
		}

	}
	exit(0);
}
