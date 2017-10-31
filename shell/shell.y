%{
#include "global.h"
#include "launchManager.h"
#include "operator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int yylex();
int yyerror(char*);

/*extern YY_FLUSH_BUFFER;
extern YYACCEPT;
extern YYABORT;
extern YYRESET;
extern YYTEST;
extern int YYLESS(int n);
*/



%}

%token CMD AND PIPE OR BACKGROUND SEPARATOR EXIT END

%left AND OR
%left PIPE SEPARATOR BACKGROUND

%start Input
%%
Input:

	| Input Line
	;

Line:
	END
	|Command END { /*displayListCommand(*/endInitStack()/*)*/, printf("\n> ");}
	;

Command:
	CMD {addToStack($1);}
	|Command AND Command {addToStack(OP_AND);}
	|Command OR Command {addToStack(OP_OR);}
	|Command PIPE Command {addToStack(OP_PIPE);}
	|Command SEPARATOR Command {addToStack(OP_SEPARATOR);}
	|Command BACKGROUND {addToStack(OP_BACKGROUND);}
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
	//signal(SIGINT, SIG_DFL);
	
	
	//res = getchar();
	//fflush(stdin);
	//while (!feof(stdin))getchar();
	
	//scanf("%c",&res);
	//fgets(buf, 30, stdin);
	scanf(" %s",buf);
	res=buf[0];
	
	//ungetc(res,stdin);
	//write(0, "a", 1);

	
	printf("res: >%c<\n", res);
	switch(res){
		case 'y':
			printf("quitte le programme...\n");
			exit(0);
		break;
		case 'n':
			//printf("\n> ");
			//fflush(stdout);
			
			//yyless(0);
			return;
		break;
		default:
			//printf("\n> ");
			//fflush(stdout);
			return;
		break;
	} 
	return;
	//change_sigint();
}


void change_sigint(void){
	signal(SIGINT, handler_sigint);

}

int main(void) {
	
	change_sigint();
	printf("> ");
	fflush(stdout);
	yyparse();
}
