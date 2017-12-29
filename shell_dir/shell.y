%{
#include "global.h"
#include "launchManager.h"
#include "operator.h"
#include "./modules/myjobs/myjobs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>

#define BLUE(m) "\033[01;34m"m"\033[0m"

int yylex();
int yyerror(char*);
char init_cwd[1024];

/*extern YY_FLUSH_BUFFER;
extern YYACCEPT;
extern YYABORT;
extern YYRESET;
extern YYTEST;
extern int YYLESS(int n);
*/



%}

%token CMD AND PIPE OR BACKGROUND SEPARATOR FLUX_WRITE FLUX_READ MY_JOBS EXIT MY_LS END BLANK

%left AND OR
%left PIPE SEPARATOR BACKGROUND
%left FLUX_READ FLUX_WRITE_OUT FLUX_WRITE_OUT_DOUBLE FLUX_WRITE_ERR FLUX_WRITE_ERR_DOUBLE FLUX_WRITE_BOTH FLUX_WRITE_BOTH_DOUBLE
%left MY_JOBS MY_LS MY_PS

%start Input
%%
Input:

	| Input Line
	;

Line:
	END
	|Command END { endInitStack(); char cwd[1024];  getcwd(cwd, sizeof(cwd)); printf(BLUE("%s > "),cwd);}
	;

Command:
	CMD {addToStack($1);}
	|MY_JOBS {printf("Je suis la commande my_jobs");}
	|Command AND Command {addToStack(OP_AND);}
	|Command OR Command {addToStack(OP_OR);}
	|Command PIPE Command {addToStack(OP_PIPE);}
	|Command SEPARATOR Command {addToStack(OP_SEPARATOR);}
	|Command BACKGROUND {addToStack(OP_BACKGROUND);}
	|Command FLUX_WRITE_OUT Command {addToStack(OP_FLUX_WRITE_OUT);}
	|Command FLUX_WRITE_OUT_DOUBLE Command {addToStack(OP_FLUX_WRITE_OUT_DOUBLE);}
	|Command FLUX_WRITE_ERR Command {addToStack(OP_FLUX_WRITE_ERR);}
	|Command FLUX_WRITE_ERR_DOUBLE Command {addToStack(OP_FLUX_WRITE_ERR_DOUBLE);}
	|Command FLUX_WRITE_BOTH Command {addToStack(OP_FLUX_WRITE_BOTH);}
	|Command FLUX_WRITE_BOTH_DOUBLE Command {addToStack(OP_FLUX_WRITE_BOTH_DOUBLE);}
	|Command FLUX_READ Command {addToStack(OP_FLUX_READ);}
	|EXIT {addToStack(OP_EXIT);}
	;
%%

int yyerror(char *s) {
	printf("%s\n", s);
	return 1;
}

void handler_sigint(int arg){
	
	char res = ' ';
	char *buf = malloc(sizeof(char)*30);
	printf("\n\nDo you really wish to quit ?\n yes=y\n no=n\n");
	signal(SIGINT, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
	
	scanf(" %s",buf);
	res=buf[0];
	free(buf);
	
	switch(res){
		case 'y':
			exit(1);
			return;
			
		break;
		case 'n':
			printf("> ");
			fflush(stdout);
			signal(SIGINT, handler_sigint);
			return;
		break;
		default:
			signal(SIGINT, handler_sigint);
			return;
		break;
	} 
	
	return;
	
}


int main(void) {
	//signal(SIGINT, SIG_IGN);
	//signal(SIGTSTP, SIG_IGN);
	
	init_list_jobs();
	
	signal(SIGINT, handler_sigint);
	
    getcwd(init_cwd,sizeof(init_cwd));
	printf(BLUE("%s > "),init_cwd);
	yyparse();
	
}
