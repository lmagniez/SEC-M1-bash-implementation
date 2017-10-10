%{
#include "global.h"
#include <stdio.h>
int yylex();
int yyerror(char*);
%}

%token CMD
%token OR PIPE AND BACKGROUND SEPARATOR
%token END

%left AND OR
%left PIPE SEPARATOR BACKGROUND

%start Input
%%
Input:

	| Input Line
	;

Line:
	END
	|Command END {}
	;

Command:
	CMD {printf("%s ", $1);}
	|Command AND Command {printf("AND");}
	|Command OR Command {printf("OR");}
	|Command PIPE Command {printf("PIPE");}
	|Command SEPARATOR Command {printf("SEPARATOR");}
	|Command BACKGROUND {printf("BACKGROUND");}
	;
%%

int yyerror(char *s) {
	printf("%s\n", s);
	return 1;
}

int main(void) {
	yyparse();
}