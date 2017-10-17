#include "launchManager.h"
newstack(char*, stack);
newstack(char*, cmdStack);
newstack(char*, operatorStack);

void addToStack(char *value) {
	char* valAdd = malloc(sizeof(value) + 1);
	memcpy(valAdd, value, sizeof(value) + 1);
	push(stack, valAdd);
}

void displayStack(void) {
	printf("Contenu de la stack %ld:\n", size(stack));
	while(!empty(stack)) {
		char *tmp = pop(stack);
		printf("%s", tmp);
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

	displayListCommand();
}

void launchCommands(void) {
	while(!empty(cmdStack)) {
		char *operator = pop(operatorStack);
		char *cmd = pop(cmdStack);

		pid_t pid = fork();

		if (pid == 0) {

		} else if (pid > 0) {
			int status;
			wait(&status);
		} else {

		}
	}
}

void displayListCommand(void) {
	printf("Affichage stack opérateur\n");

	while(!empty(operatorStack)) {
		printf("%s \n", pop(operatorStack));
	}

	printf("Affichage stack cmd\n");

	while(!empty(cmdStack)) {
		printf("%s\n", pop(cmdStack));
	}
}
