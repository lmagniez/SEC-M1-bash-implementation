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
		//char *operator = pop(operatorStack);
		char *cmd = pop(cmdStack);
		char** commandArray = getCommandsArray(cmd);

		pid_t pid = fork();

		if (pid == 0) {
			execv(commandArray[0], &commandArray[1]);
			perror("Error exec");
			exit(errno);
		} else if (pid > 0) {
			int status;
			wait(&status);

		} else {
			perror("Error on fork");
			exit(errno);
		}
	}
}

char **getCommandsArray(char* commandLine) {
	printf("ligne de commande : %s", commandLine);
	char *test = strtok(commandLine, " ");
	char *firstValue = malloc(strlen(CMD_DIR) + strlen(test) + 1);
	char **array = malloc(sizeof(char*) * ARRAY_SIZE_DEFAULT);
	int index = 1;
	int arraySize = ARRAY_SIZE_DEFAULT;
	strcpy(firstValue, CMD_DIR);
	strcat(firstValue, test);
	array[0] = firstValue;

	while(test != NULL) {
		test = strtok(NULL, " ");
		if (index == arraySize) {
			arraySize*=2;
			array = realloc(array, sizeof(char*) * arraySize);
		}
		array[index] = test;
		index++;
	}

	//display test
	int i;
	for (i = 0; i < index; i++) {
		printf("value : %s", array[i]);
	}

	if (array[i] == NULL) {
		printf("NULL");
	}

	free(commandLine);

	return array;
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
