#ifndef LOCALMEMORY
#define LOCALMEMORY

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct memory {
	char * id;
	char * val;
	struct memory * next;
} MemoryList;

static MemoryList *localMemory = NULL;

int estPresent(char *id);
void remplacerValeur(char * id, char * valeur);
void addLocalMemory(char * id, char * val);
void removeLocalMemory(char *id);
void printLocalMemory();
char* getFromLocalMemory(char *id);

#endif