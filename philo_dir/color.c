#include <stdio.h>

#define couleurnormale()  		  printf("\033[0m");
#define couleurRouge(message)     printf("\033[01;31m%s\n",message);couleurnormale();
#define couleurVert(message)      printf("\033[01;32m%s\n",message);couleurnormale();
#define couleurJaune(message) 	  printf("\033[01;33m%s\n",message);couleurnormale();
#define couleurBleuFonce(message) printf("\033[01;34m%s\n",message);couleurnormale();
#define couleurGris(message)      printf("\033[01;35m%s\n",message);couleurnormale();
#define couleurClair(message)     printf("\033[01;36m%s\n",message);couleurnormale();
