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

[ ]*"exit"[ ]* {
	return EXIT;
}

([a-zA-Z"'\\/0-9.]*[\ -]*)* {
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
