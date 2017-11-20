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

%token CMD AND PIPE OR BACKGROUND SEPARATOR FLUX_WRITE FLUX_READ MY_JOBS EXIT MY_LS END

%left AND OR
%left PIPE SEPARATOR BACKGROUND
%left FLUX_READ FLUX_WRITE
%left MY_JOBS MY_LS

%start Input
%%
Input:

	| Input Line
	;

Line:
	END
	|Command END { /*displayListCommand(*/endInitStack()/*)*/, printf("> ");}
	;

Command:
	CMD {addToStack($1);}
	|MY_JOBS {printf("Je suis la commande my_jobs");}
	|MY_LS {printf("Je suis la commande my_ls");}
	|MY_PS {printf("Je suis la commande my_ps");}
	|Command AND Command {addToStack(OP_AND);}
	|Command OR Command {addToStack(OP_OR);}
	|Command PIPE Command {addToStack(OP_PIPE);}
	|Command SEPARATOR Command {addToStack(OP_SEPARATOR);}
	|Command BACKGROUND {addToStack(OP_BACKGROUND);}
	|Command FLUX_WRITE Command {addToStack(OP_FLUX_WRITE);}
	|Command FLUX_READ Command {addToStack(OP_FLUX_READ);}
	|EXIT {addToStack(OP_EXIT);}
	;
%%

int yyerror(char *s) {
	printf("%s\n", s);
	return 1;
}


int main(void) {
	signal(SIGINT, SIG_IGN);
	printf("> ");
	fflush(stdout);
	yyparse();
	
}
