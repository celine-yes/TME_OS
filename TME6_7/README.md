# TME 6-7

## 1. MISE EN OEUVRE AVEC UN TAMPON A UNE SEULE CASE

#### 1.1

Il faut créer un autre sémaphore pour garantir l'exclusion mutuelle de nb_recepteurs.

Les initialisations des sémaphores et des variables partagées sont les suivantes :

Initialisation du sémaphore EMET à 1, ce qui permettra aux émetteurs de commencer à écrire immédiatement s'il y a une case vide dans le tampon.
Initialisation des sémaphores du tableau RECEP à 0 pour bloquer tous les récepteurs jusqu'à ce qu'il y ait un message à lire.
Initialisation du compteur partagé nb_recepteurs à 0 pour indiquer que aucun récepteur n'a encore lu le premier message produit.

#### 1.2

On utilise un tableau de sémaphore RECEP[1..NR], dont chaque case est augmentée de 1 lors de l'émission d'un message plutôt qu'un unique sémaphore RECEP pour éviter de bloquer tous les récepteurs quand un récepteur n'a plus rien à lire, en effet cela permet à plusieurs recepteurs de travailler en parallèle.

Les émetteurs sont prévenus de la disponibilité de la case lorsque tous les recepteurs  ont lu le message dans la case, donc si nb_recepteurs = NR.    


## 2. MISE EN OEUVRE AVEC UN TAMPON A NMAX CASES

#### 2.1

- Il faudrait déclarer un tableau de sémaphore représentant les cases du tampon, tous initialisés à 1.
- Un sémaphore EMET initialisé au nombre d'émetteur
- Un tableau de sémaphore RECEP de taille NR
- 4 variables partagées id, ir, un tableau de message représentant les messages dans chaque case du tampon, et un tableau nb_recepteurs représentant le nombre de récepteur qui ont lu le message dans la case en question.
- 4 semaphores garantissant l'exclusion mutuelle de chaque variables partagées.

#### 2.2
