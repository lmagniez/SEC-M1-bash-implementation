#ifndef MY_PIPE
#define MY_PIPE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



void my_pipe(int begin, int end);
void swap_tube();
void my_pipe(int begin, int end);
void prepare_pipe(char *operator);
void traitement_pipe_fils();
void traitement_pipe_pere();

#endif
