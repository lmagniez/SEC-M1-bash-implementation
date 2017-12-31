#include "localMemory.h"

int estPresent(char *id) {
	MemoryList *tmp = localMemory;

	while (tmp != NULL) {
		if (strcmp(tmp->id, id) == 0) {
			return 1;
		}

		tmp = tmp->next;
	}

	return 0;
}

void remplacerValeur(char * id, char * valeur) {
	MemoryList *tmp = localMemory;

	while (tmp != NULL) {
		if (strcmp(tmp->id, id) == 0) {
			free(tmp->val);
			tmp->val = valeur;

			return;
		}

		tmp = tmp->next;
	}
}

void addLocalMemory(char * id, char * val) {
	if (estPresent(id)) {
		char *value = malloc(strlen(val) + 1);
		strcpy(value, val);
		remplacerValeur(id, value);

		return;
	}

	char *idTmp = malloc(strlen(id) + 1);
	char *valTmp = malloc(strlen(val) + 1);
	strcpy(idTmp, id);
	strcpy(valTmp, val);

	if (localMemory == NULL) {
		localMemory = malloc(sizeof(MemoryList));
		localMemory->id = idTmp;
		localMemory->val = valTmp;
		localMemory->next = NULL;
	} else {
		MemoryList *tmp = malloc(sizeof(MemoryList));
		tmp->id = idTmp;
		tmp->val = valTmp;
		tmp->next = localMemory;
		localMemory = tmp;
	}
}


void removeLocalMemory(char *id) {
	MemoryList *tmp = localMemory;
	MemoryList *caseAvant = localMemory;

	if (tmp == NULL) {
		return;
	}

	if (strcmp(id, tmp->id) == 0) {
		localMemory = tmp->next;
		free(tmp->id);
		free(tmp->val);
		free(tmp);
		return;
	}

	while(tmp != NULL) {
		if (strcmp(id, tmp->id) == 0) {
			caseAvant->next = tmp->next;
			free(tmp->id);
			free(tmp->val);
			free(tmp);
			break;
		}

		caseAvant = tmp;
		tmp = tmp->next;
	}
}

void printLocalMemory() {
	MemoryList *tmp = localMemory;

	while(tmp != NULL) {
		printf("id : %s valeur : %s\n", tmp->id, tmp->val);

		tmp = tmp->next;
	}
}

char* getFromLocalMemory(char* id) {
	MemoryList *tmp = localMemory;

	while(tmp != NULL) {
		if (strcmp(tmp->id, id) == 0) {
			return tmp->val;
		}

		tmp = tmp->next;
	}

	char * res = malloc(1);
	*res = '\0';

	return res;
}