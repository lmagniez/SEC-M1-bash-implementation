%{
#include "global.h"
#include "y.tab.h"
#include <stdio.h>
%}

%%
[ ]*"exit"[ ]* {
	return EXIT;
}

([a-z]*[\ -]*)* {
	yylval=yytext;
	return (CMD);
}
"||" {
	return(OR);
}
"|" {
	return(PIPE);
}
"&" {
	return(BACKGROUND);
}
"&&" {
	return(AND);
}
";" {
	return(SEPARATOR);
}

"\n" {
	return(END);
}

%%
