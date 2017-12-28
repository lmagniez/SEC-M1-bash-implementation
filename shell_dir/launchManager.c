#include "launchManager.h"


newstack(char*, stack);
newstack(char*, cmdStack);
newstack(char*, operatorStack);

int pid_pere = 0;
int pid_fils = 0;
char *last_cmd;
int stop_happened = 0;



static void handler_sigchld_bg (int sig, siginfo_t *siginfo, void *context)
{
	
	printf("handler sigchld\n");
	int status = siginfo->si_status;
	int pid = siginfo->si_pid;
	int id_job = get_id_job(pid);
	
	afficher_job();
		
	if (WIFEXITED(status)) {
		int returnCode = WEXITSTATUS(status);
		int isfg;
		if(id_job!=-1){
			isfg = is_fg(id_job);
			remove_job(id_job);
		}
		printf("fg: %d\n", isfg);
		if(!isfg){
			printf ("Le processus [%ld] s'est bien terminé (code: %d)\n",
			(long)siginfo->si_pid, returnCode);
		}
		
	}
	else if(WIFSIGNALED(status)){
		printf(" by signal %d\n", WTERMSIG(status));
		if(WTERMSIG(status) == 20){
			set_fg(id_job, 2);
		}
		else{
			int isfg = 0;
			if(id_job!=-1){
				isfg = is_fg(id_job);
				remove_job(id_job);
			}
			if(!isfg){
				printf ("Le processus [%ld] s'est terminé (code: %d)\n",
				(long)siginfo->si_pid, status);
			}
			
		}
	
	}
	else if (WIFSTOPPED(status)) {
		printf("stopped by signal %d\n", WSTOPSIG(status));
	}
	
	
}

void launch_sigaction_pere(){	
	struct sigaction act;
	memset (&act, '\0', sizeof(act));
	act.sa_sigaction = &handler_sigchld_bg;
	act.sa_flags = SA_SIGINFO;			 
	if (sigaction(SIGCHLD, &act, NULL) < 0) {
		perror ("sigaction");
		return;
	}

}

void addToStack(char *value) {
	char* valAdd = malloc(sizeof(char)*strlen(value)+1);
	memcpy(valAdd, value, sizeof(char)*strlen(value)+1);
	push(stack, valAdd);
}

void displayStack(void) {
	printf("Contenu de la stack %ld:\n", size(stack));
	while(!empty(stack)) {
		char *tmp = pop(stack);
		printf("%s", tmp);
		free(tmp);
	}
}

void endInitStack(void) {
	while (!empty(stack)) {
		char *val1, *val2;

		val1 = pop(stack);

		if (isOperator(val1)) {
			if (!empty(stack)) {
				val2 = pop(stack);
				if (isOperator(val2)) {
					push(operatorStack, val2);
				} else {
					push(cmdStack, val2);
				}
			}

			push(operatorStack, val1);
		} else {
			push(cmdStack, val1);
		}
	}
	//getCommandsArray(pop(cmdStack));
	//displayListCommand();
	launchCommands();
}


void launchCommands(void) {
	while(!empty(cmdStack)) {
		char *operator = ((!empty(operatorStack)) ? pop(operatorStack) : NULL);
		char *cmd = pop(cmdStack);
		char** commandArray = getCommandsArray(cmd);
		last_cmd = commandArray[0];
		stop_happened = 0;
		
		int i=1;
		
		//detect the * and replace with the corresponding elements 
		if(commandArray!=NULL){
			while(commandArray[i]){
				if(strchr(commandArray[i],'*')||strchr(commandArray[i],'?')||strchr(commandArray[i],'[')){
					char ** res = get_elements(commandArray[i]);
					
					if(*res!=NULL){
						char **new_cmd = replace_cmdarray(commandArray, i, res);
						destroyCommandsArray(commandArray);
						commandArray = new_cmd;
					}
				}
				i++;
			}
		}
		prepare_pipe(operator);
		pid_pere = getpid();
		
		pid_t pid = fork();

		//son pid: exec the command
		if (pid == 0) {
			
			if(operator != NULL){
				if(isBackgroundOperator(operator)){
					
				}
			}
			
			traitement_pipe_fils();
			
			//>>>>>>>>>>MYFG
			if(strcmp(MYFG,commandArray[0])==0){
				
				int pid_fg;
				if(commandArray[1] == NULL){
					exit(1);
				}
				else{
					int id_job = atoi(commandArray[1]);
					if(!has_id(id_job)){
						printf("existe pas\n");
						exit(1);
					}
					else{
						pid_fg = get_pid_from_id_job(id_job);
						kill(pid_fg, SIGCONT);
					}
					int status;
					//waitpid(pid_fg, &status, WUNTRACED);
					exit(0);
				}
				
				exit(0);
			}
			else if(strcmp(MYJOB,commandArray[0])==0){
				afficher_job();
				exit(0);
			}
			else if(strcmp(MYLS,commandArray[0])==0){
				execvpe(MYLS_PATH, commandArray, environ);
			}
			else if(strcmp(MYPS,commandArray[0])==0){
				execvpe(MYPS_PATH, commandArray, environ);
			}
			else if(strcmp(MYCD,commandArray[0])==0){
				//done in the father
				exit(0);
			}
			else{
				execvpe(commandArray[0], commandArray, environ);
			}
			perror("Error exec");
			exit(errno);
		
		} 
		//father pid: wait for the son
		else if (pid > 0) {
			
			signal(SIGTSTP, SIG_IGN);
			pid_fils = pid;
			
			
			int status;
			
			//launch all the signal
			//>>>launch_sigaction_pere();
			traitement_pipe_pere();
			
			if(strcmp(MYCD,commandArray[0])==0){
				if(commandArray[1] == NULL){
					exit(1);
				}
				else{
					int res = cd(commandArray[1]);
					if(res == 0) printf("switched to the %s directory\n",commandArray[1]);
				}
			}
			
			//& operator
			if(operator != NULL){
				if(isBackgroundOperator(operator)){
					signal(SIGCHLD, SIG_DFL);
					ajout_job(pid, commandArray[0], 0);
					//afficher_job();
					//int status;
					//waitpid(pid, &status, WNOHANG);
					
				}
				
			}
			else{
				ajout_job(pid_fils, commandArray[0], 1);
				int status;
				waitpid(pid, &status, WUNTRACED);//for sleep too
				
				if (WIFEXITED(status)) {
					int returnCode = WEXITSTATUS(status);
					//printf("exit with return code %d\n", returnCode);

					if (operator != NULL) {
						
						if (isAndOperator(operator) && returnCode > 0) {
							unStack();
						} else if (isOrOperator(operator) && returnCode == 0) {
							unStack();
						} 
						
					}
				}
				destroyCommandsArray(commandArray);
				
			}
			signal(SIGTSTP, SIG_DFL);

		} else {
			perror("Error on fork");
			exit(errno);
		}
	}
}

void unStack() {
	while (!empty(cmdStack)) {
		free(pop(cmdStack));
		
		if (!empty(operatorStack)) {
			char *operator = pop(operatorStack);

			if(isSeparatorOperator(operator)) {
				free(operator);
				break;
			}
		}
	}
}

char **getCommandsArray(char* commandLine) {
	char *test = strtok(commandLine, " ");
	char *firstValue = malloc(strlen(test) + 1);
	char **array = malloc(sizeof(char*) * ARRAY_SIZE_DEFAULT);
	int index = 1;
	int arraySize = ARRAY_SIZE_DEFAULT;
	strcpy(firstValue, test);
	array[0] = firstValue;

	while(test != NULL) {
		test = strtok(NULL, " ");
		if (index == arraySize) {
			arraySize*=2;
			array = realloc(array, sizeof(char*) * arraySize);
		}

		if (test != NULL) {
			char *value = malloc(strlen(test) + 1);
			strcpy(value, test);
			array[index] = value;
		} else {
			array[index] = NULL;
		}
		index++;
	}

	free(commandLine);

	return array;
}

void destroyCommandsArray(char **commandArray) {
	char **index = commandArray;

	while (*index != NULL) {
		free(*index);
		index++;
	}

	free(commandArray);
}

void freeList(){
	printf("FREE LIST \n");
	while(!empty(operatorStack)) {
		char * operator = pop(operatorStack);
		free(operator);
	}
	while(!empty(cmdStack)) {
		char * cmd = pop(cmdStack);
		free(cmd);
	}
	while(!empty(stack)) {
		char *tmp = pop(stack);
		free(tmp);
	}
}

void displayListCommand(void) {
	printf("Affichage stack opérateur\n");

	while(!empty(operatorStack)) {
		char * operator = pop(operatorStack);
		printf("%s \n", operator);
		free(operator);
	}

	printf("Affichage stack cmd\n");

	while(!empty(cmdStack)) {
		char * cmd = pop(cmdStack);
		printf("%s\n", cmd);
		free(cmd);
	}
}
