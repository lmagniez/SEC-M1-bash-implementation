#include "color.c"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define nb_tour 5

char string[]="PHILOSOPHE \t";
char arrive[]=" : SE MET A TABLE ET PENSE";
char faim[]=" : FAIM";
char mange[]=" : MANGE";
char pense[]=" : PENSE";
char quitte[]=" : QUITTE LA TABLE";

typedef enum v_etat{
  PENSE,
  AFAIM,
  MANGE,
}value_etat;

int nombrePhilosophe;

sem_t all_a_table; 
sem_t mutex; // exclusion mutuelle
sem_t * philosophe; //semaphore par philosophe
value_etat * etat; //etat par philosophe
pthread_t * philosopheThreadTab; //thread par philosophe

// --------------------------------
// ------------ TOOL --------------
// --------------------------------
char * intToChar(int number){
  char * str = malloc(sizeof(char)*10);
  sprintf(str, "%d", number);
  return str;
}

// --------------------------------
// ------------ INIT --------------
// --------------------------------
void init(int numPhilosophe){
  nombrePhilosophe = numPhilosophe;
  philosophe = malloc(sizeof(sem_t)*numPhilosophe);
  etat = malloc(sizeof(value_etat)*numPhilosophe); 
  philosopheThreadTab = malloc(sizeof(pthread_t)*numPhilosophe); 

  for (int i=0;i<numPhilosophe;i++){
    sem_init(&philosophe[i], 0, 1);
    etat[i] = PENSE;
  }

  sem_init(&mutex, 0, 1);
  sem_init(&all_a_table,0,0);
}

// --------------------------------
// ------------- V/P --------------
// --------------------------------
void P(sem_t * mut){
  sem_wait(mut);
}

void V(sem_t * mut){
  sem_post(mut);
}

// --------------------------------
// ----------- ACTION -------------
// --------------------------------
void penser(int currentPhilo){
  char * message = malloc(256);
  char * num = intToChar(currentPhilo);

  strcpy(message,string);

  strcat(message,num);

  strcat(message,pense);

  couleurJaune(message);

  free(message);
  free(num);
}

void manger(int currentPhilo){
  char * message = malloc(256);
  char * num = intToChar(currentPhilo);

  strcpy(message,string);

  strcat(message,num);

  strcat(message,mange);

  couleurBleuFonce(message);

  free(message);
  free(num);
}

void afaim(int currentPhilo){
  char * message = malloc(256);
  char * num = intToChar(currentPhilo);

  strcpy(message,string);

  strcat(message,num);

  strcat(message,faim);

  couleurClair(message);

  free(message);
  free(num);
}

void arriver(int currentPhilo){
  char * message = malloc(256);
  char * num = intToChar(currentPhilo);

  strcpy(message,string);

  strcat(message,num);

  strcat(message,arrive);

  couleurVert(message);

  free(message);
  free(num);
}

void quitter(int currentPhilo){
  char * message = malloc(256);
  char * num = intToChar(currentPhilo);

  strcpy(message,string);

  strcat(message,num);

  strcat(message,quitte);

  couleurRouge(message);

  free(message);
  free(num);
}


// --------------------------------
// -------- GAUCHE/DROITE ---------
// --------------------------------
int gauche(int num){
  return (((num)-1+nombrePhilosophe)%nombrePhilosophe);
}

int droite(int num){
  return (((num)+1+nombrePhilosophe)%nombrePhilosophe);
}

// --------------------------------
// ---------- BAGUETTES -----------
// --------------------------------
void testBaguettes(int num){
  if((etat[num] == AFAIM ) && (etat[gauche(num)] != MANGE ) && (etat[droite(num)] != MANGE )){
      
      etat[num] = MANGE;
      manger(num);
      
      V(&(philosophe[num]));
   } 
}

void prendre_baguettes(int num){
   P(&mutex); 

   etat[num] = AFAIM; 
   afaim(num);
   
   testBaguettes(num);
   
   V(&mutex);
   P(&(philosophe[num]));
}

void poser_baguettes(int num){
   P(&mutex); 

   etat[num] = PENSE;
   penser(num);
   
   testBaguettes(gauche(num)); 
   testBaguettes(droite(num));
   V(&mutex);
}

// --------------------------------
// ---------- PHILOSOPHE ----------
// --------------------------------
void * action_philosophe(void * arg){
  int number = *(int*)arg;

  etat[number] = PENSE;
  arriver(number);

  sem_wait(&all_a_table);
  sem_post(&all_a_table);

  for(int i = 0 ; i < nb_tour ; i++){

    prendre_baguettes(number);

    poser_baguettes(number);
  }

  quitter(number);

  free(arg);
  (void) arg;
  pthread_exit(NULL);
}

int * newInt(int a){
  int num = a;
  int * new = malloc(sizeof(num));
  *new = num;
  return new;
}

int main(int argc, char* argv[]){
    if(argc < 2){
      couleurRouge("Manque d'argument : philo <int>");
      exit(0);
    }

    int numPhilosophe = atoi(argv[1]);
    init(numPhilosophe);
    
    for(int a = 0;a < numPhilosophe;a++){
        if(pthread_create(&philosopheThreadTab[a], NULL, action_philosophe,newInt(a)) == -1) {
          couleurRouge("pthread_create()");
          exit(0);
        }
    }

    sem_post(&all_a_table);

    for (int i = 0; i < numPhilosophe; i++){
        if (pthread_join(philosopheThreadTab[i], NULL) != 0){
           couleurRouge("pthread_join()");
        }
    }
    return 0;
}	
