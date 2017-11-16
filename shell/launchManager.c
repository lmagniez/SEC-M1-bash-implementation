#include "launchManager.h"

newstack(char*, stack);
newstack(char*, cmdStack);
newstack(char*, operatorStack);


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
		
		int i=1;
		
		//detect the * and replace with the corresponding elements 
		if(commandArray!=NULL){
			while(commandArray[i]){
				if(strchr(commandArray[i],'*')){
					char ** res = get_elements(commandArray[i]);
					/*
					int cpt=0;
					while(res[cpt]!=NULL){
						printf("elt: %s \n",res[cpt++]);
					}
					*/
					
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
		pid_t pid = fork();

		if (pid == 0) {
			
			traitement_pipe_fils();
			
			execvpe(commandArray[0], commandArray, environ);
			perror("Error exec");
			exit(errno);
			
		} else if (pid > 0) {
			
			traitement_pipe_pere();
			
			int status;
			wait(&status);
			destroyCommandsArray(commandArray);
			
			
			if (WIFEXITED(status)) {
				int returnCode = WEXITSTATUS(status);

				if (operator != NULL) {
					
					if (isAndOperator(operator) && returnCode > 0) {
						unStack();
					} else if (isOrOperator(operator) && returnCode == 0) {
						unStack();
					}
				}
			}

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
