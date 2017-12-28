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
sem_t mutex; // exclusion mutuelle
sem_t * philosophe; //semaphore par philosophe
value_etat * etat; //etat par philosophe
pthread_t * philosopheThreadTab; //thread par philosophe
int * mangertab;

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
  mangertab = malloc(sizeof(int)*numPhilosophe); 
  philosopheThreadTab = malloc(sizeof(pthread_t)*numPhilosophe); 

  for (int i=0;i<numPhilosophe;i++){
    sem_init(&(philosophe[i]), 0, 1);
    etat[i] = PENSE;
    mangertab[i]=0;
  }

  sem_init(&mutex, 0, 1);

}

//---------------------------------
//----------- DESTROY -------------
//---------------------------------
void destroy(){
  sem_destroy(&mutex);
  for(int i = 0 ; i < nombrePhilosophe;i++){
    sem_destroy(&(philosophe[i]));
  }
  free(philosophe);
  free(etat);
  free(philosopheThreadTab);
}

// --------------------------------
// ------------- V/P --------------
// --------------------------------
void P(sem_t * mut){
  int erreur = sem_wait(mut);
  if(erreur == -1) {
    couleurRouge("sem_wait()");
    exit(0);
  }
}

void V(sem_t * mut){
  int erreur = sem_post(mut);
  if(erreur == -1) {
    couleurRouge("sem_post()");
    exit(0);
  }
}

// --------------------------------
// ----------- ACTION -------------
// --------------------------------
void couleur(char * message,int num){
  switch(num%5){
    case 0:
      couleurVert(message);
    break;
    case 1:
      couleurJaune(message);
    break;
    case 2:
      couleurBleuFonce(message);
    break;
    case 3:
      couleurGris(message);
    break;
    case 4:
      couleurClair(message);
    break;
  }
}



void penser(int currentPhilo){
  char * message = malloc(256);
  char * num = intToChar(currentPhilo);

  strcpy(message,string);

  strcat(message,num);

  strcat(message,pense);

  couleur(message,currentPhilo);

  free(message);
  free(num);
}

void manger(int currentPhilo){
  char * message = malloc(256);
  char * num = intToChar(currentPhilo);

  strcpy(message,string);

  strcat(message,num);

  strcat(message,mange);

  couleur(message,currentPhilo);

  free(message);
  free(num);
}

void afaim(int currentPhilo){
  char * message = malloc(256);
  char * num = intToChar(currentPhilo);

  strcpy(message,string);

  strcat(message,num);

  strcat(message,faim);

  couleur(message,currentPhilo);

  free(message);
  free(num);
}

void arriver(int currentPhilo){
  char * message = malloc(256);
  char * num = intToChar(currentPhilo);

  strcpy(message,string);

  strcat(message,num);

  strcat(message,arrive);

  couleur(message,currentPhilo);

  free(message);
  free(num);
}



void quitter(int currentPhilo){
  char * message = malloc(256);
  char * num = intToChar(currentPhilo);

  strcpy(message,string);

  strcat(message,num);

  strcat(message,quitte);

  couleur(message,currentPhilo);

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
      mangertab[num] = mangertab[num] + 1; 
      V(&(philosophe[num]));
   } 
}

void testBaguetteVoisinGauche(int num){
  if((etat[num] == AFAIM ) && (etat[gauche(num)] != MANGE )){
      V(&(philosophe[num]));
      etat[num] = MANGE;
      manger(num);
      mangertab[num] = mangertab[num] + 1; 
   } 
}

void testBaguetteVoisinDroite(int num){
  if((etat[num] == AFAIM ) && (etat[droite(num)] != MANGE )){
      V(&(philosophe[num]));
      etat[num] = MANGE;
      manger(num);
      mangertab[num] = mangertab[num] + 1; 
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
   
   testBaguetteVoisinGauche(gauche(num)); 
   testBaguetteVoisinDroite(droite(num));
   V(&mutex);
}

// --------------------------------
// ---------- PHILOSOPHE ----------
// --------------------------------
void * action_philosophe(void * arg){
  int number = *(int*)arg;

  arriver(number);

  for(int i = 0 ; i < nb_tour ; i++){
    sleep(1);
    prendre_baguettes(number);
    sleep(1);
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

    for (int i = 0; i < numPhilosophe; i++){
        if (pthread_join(philosopheThreadTab[i], NULL) != 0){
           couleurRouge("pthread_join()");
        }
    }

    destroy();

    return 0;
}	
