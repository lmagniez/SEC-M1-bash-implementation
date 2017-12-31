## Présentation ##

Projet de sec : réalisation d'un shell par groupe de 3

## Développement ##

- c
 
## Répartition des tâches 

### Vincent
- Myps
- Problème des philosophes
- Commande cd
- Création des makefile du projet

### Florent 
- Implémentation squelette du parseur lex/yacc (grammaires, règles...)
- opérateurs && || ;
- Gestion de la mémoire locale

### Loïck
- myls
- insertion des différents modules dans le parseur 
- pipe et redirections
- signaux
- caractère joker
- mystatus

## Les petis plus

###Myps
- State avec precisions :  +;N;l;L;s;<
- Enregsitrement d'une variable dans une autre variable local "set a=$c"

## Problèmes rencontrés
### signaux
- Lorsque qu'on demande de quitter et que l'on indique non, le prochain caractère est "mangé" par le parseur, ce qui fausse la commande et la commande d'apres de ne se fait pas.
- Sigchld a la mort d'un processus en arrière provoque le plantage du parseur flex.
	- Problème pour le passage des processus en arrière plan/premier plan
	- Problème pour la reprise des processus
- Mémoire partagée non gérée
- 


