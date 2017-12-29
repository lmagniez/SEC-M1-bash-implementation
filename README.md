## Présentation ##

Projet de sec : réalisation d'un bash

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
- Gestion de la mémoire


### Loïck
- myls
- insertion des différents modules dans le parseur 
- pipe et redirections
- signaux
- caractère joker

## Problèmes rencontrés
### signaux
- Lorsque qu'on demande de quitter et que l'on indique non, le prochain caractère de la commande suivante est "mangé" par le parseur, ce qui fausse la commande
- Sigchld a la mort d'un processus en arrière provoque le plantage du parseur flex.
...


