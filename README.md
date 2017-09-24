## Présentation ##

Projet de sec : réalisation d'un bash

## Développement ##

- c

## Réglement ##

Utilisation du karma commit pour plus de compréhension sur la forge : [Karma] (http://karma-runner.github.io/1.0/dev/git-commit-msg.html).


## Lancement du projet ##
``gcc -o shell shell.c``
``./shell``

## Commande PS -AUX ##
- ./proc/[pid] : Nom Dossier = Numero du processus
- ./proc/[pid]/cmdline : commande d'execution -> colonne "Commande"
- ./proc/[pid]/stat : 
	- pid : %d (1)
	- state : %c (3)
	- starttime : %llu (22)
