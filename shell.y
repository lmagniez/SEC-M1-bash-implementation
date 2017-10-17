%{
#include "global.h"
#include "launchManager.h"
#include "operator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex();
int yyerror(char*);

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

int main(void) {
	printf("> ");
	yyparse();
}