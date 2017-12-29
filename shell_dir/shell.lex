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

[ ]*"exit"[ ]* {
	return EXIT;
}



(([^><&|;\n])*[^><&|;\n2]) {
	yylval=yytext;
	return (CMD);
}

(([\ ])+) {
	return (BLANK);
}


"2>>" {
	return(FLUX_WRITE_ERR_DOUBLE);
}

"2>" {
	return(FLUX_WRITE_ERR);
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

">>" {
	return(FLUX_WRITE_OUT_DOUBLE);
}

">>&" {
	return(FLUX_WRITE_BOTH_DOUBLE);
}

">&" {
	return(FLUX_WRITE_BOTH);
}



"<" {
	return(FLUX_READ);
}

">" {
	return(FLUX_WRITE_OUT);
}

%%
