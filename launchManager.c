#include "stack.h"
#include "launchManager.h"
#include "operator.h"

newstack(char*, stack);

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

static void initializeCommandStruct(CommandStruct* cmdStruct) {
	cmdStruct->cmd1 = NULL;
	cmdStruct->cmd2 = NULL;
	cmdStruct->operator = NULL;
	cmdStruct->transitionOperator = NULL;
	cmdStruct->nextCmd = NULL;
}

CommandStruct* createListCommand(void) {
	//displayStack();
	CommandStruct* commandStruct = malloc(sizeof(CommandStruct));
	initializeCommandStruct(commandStruct);
	CommandStruct* head = commandStruct;
	CommandStruct* currPos = commandStruct;

	int operatorTest = 0;
	while(!empty(stack)) {
		char *value = pop(stack);

		if (isOperator(value) && !operatorTest) {
			operatorTest = 1;
			if (currPos->operator == NULL) {
				currPos->operator = value;
			} else {
				commandStruct = malloc(sizeof(CommandStruct));
				initializeCommandStruct(commandStruct);
				commandStruct->operator = value;
				head->nextCmd = commandStruct;
			}
		} else if(operatorTest && isOperator(value)) { //creation de la transition transition
			CommandStruct* transition = malloc(sizeof(CommandStruct));
			initializeCommandStruct(transition);
			transition->transitionOperator = value;
			transition->nextCmd = head->nextCmd;
			head->nextCmd = transition;
		} else { //cas normal
			operatorTest = 0;
			if (currPos->cmd2 == NULL) {
				currPos->cmd2 = value;
			} else if (currPos->cmd1 == NULL) {
				currPos->cmd1 = value;
			} else if (head->cmd2 == NULL) {
				head->cmd2 = value;
			} else if (head->cmd1 == NULL) {
				head->cmd1 = value;
			}//voir si il ne manque pas un cas
		}
	}

	return head;
}

void displayListCommand(CommandStruct *cmd) {
	for(;cmd != NULL; cmd = cmd->nextCmd) {
		if (cmd->transitionOperator == NULL) {
			printf("Cmd 1 : %s op : %s Cmd2 : %s ", cmd->cmd1, cmd->operator, cmd->cmd2);
		} else {
			printf("Transition : %s ", cmd->transitionOperator);
		}
	}

	printf("\n");
}
