%{
#include "global.h"
#include "y.tab.h"
#include <stdio.h>
%}

%%
"myJobs" {
	return(MY_JOBS);
}

"myLs" {
	return(MY_LS);
}

"myPs" {
	return(MY_PS);
}

"set "([a-zA-Z0-9])+([=])+([a-zA-Z0-9]+) {
	yylval=yytext;
	return(SET);
}

"set"[ ]* {
	return (SET_DISPLAY);
}

"unset"[ ]+"$"([a-zA-Z0-9])+ {
	yylval=yytext;
	return(UNSET);
}

[ ]*"exit"[ ]* {
	return EXIT;
}

([$]*[a-zA-Z"'\\/0-9.\*\?\[\]+$]*[\ -]*)* {
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

">" {
	return(FLUX_WRITE);
}

"<" {
	return(FLUX_READ);
}

%%
