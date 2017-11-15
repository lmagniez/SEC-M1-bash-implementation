#include "mypipe.h"
#include "../operator.h"

int begin = 0;
int end = 0;
int pipe_ouvert = 0;
int tube_entree[]={-1,-1};
int tube_sortie[]={-1,-1};
int old_in, old_out;
int new_in, new_out;


void swap_tube(){
	int tmp[] = {0,0};
	tmp[0]=tube_entree[0];
	tmp[1]=tube_entree[1];
	tube_entree[0]=tube_sortie[0];
	tube_entree[1]=tube_sortie[1];
	tube_sortie[0]=tmp[0];
	tube_sortie[1]=tmp[1];
	
	write(tube_entree[0],"\n",1);
	
}

void my_pipe(int begin, int end){
	//do not change the stdin when this is the first pipe
	if(!begin){
		old_in = dup(0);
		close(0);
		new_in = dup(tube_entree[0]);
	}
	
	//do not change the stdou when this is the last pipe
	if(!end){
		old_out = dup(1);
		close(1);
		new_out = dup(tube_sortie[1]);
	}
	
}


void prepare_pipe(char *operator){
	//before launching fork, deal with the pipe
	begin = 0;
	end = 0;
	//swapping the tube allows us to do multiple pipe by putting the reading into the lecture
	if(pipe_ouvert){
		swap_tube();
	}
	
	if(operator!=NULL){
		if(isPipeOperator(operator)){
			pipe_ouvert = 1;
			//open the pipe if it doesn't exists
			if(write(tube_entree[1],"test",0)<0){
				begin = 1;
				if(pipe(tube_entree)<0){
					perror("pipe error");
					exit(1);
				}
			}
			
		}
		//end reached: next operator is not char
		else
			if(pipe_ouvert)end = 1;
	}
	//end reached: no more operator
	else
		if(pipe_ouvert)	
			end = 1;
	
	//ouvre le pipe de sortie (a chaque iteration)
	if(pipe_ouvert){
		if(pipe(tube_sortie)<0){
			perror("pipe error");
			exit(1);
		}
	}
}

void traitement_pipe_fils(){
	if(pipe_ouvert){
		my_pipe(begin,end);
		close(tube_entree[0]);
		close(tube_entree[1]);
	}
}

void traitement_pipe_pere(){
	if(pipe_ouvert){
		//close the first pipe, will be replaced with another one if there's another |
		close(tube_entree[0]);
		close(tube_entree[1]);
		//end reached, we close the second pipe
		if(end){
			close(tube_sortie[0]);
			close(tube_sortie[1]);
			pipe_ouvert= 0;
		}
	}
}


