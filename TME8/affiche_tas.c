#include <stdio.h>
#include "affiche_tas.h"
#include <ctype.h>

void afficher_tas(){
    int i, j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 16; j++) {
	    printf("%4d", j + 16*i);
        }
        printf("\n");
        for (j = 0; j < 16; j++) {
	    printf("%4d", tas[j + 16*i]);
        }
        printf("\n");
        for (j = 0; j < 16; j++) {
	    if (isalnum(tas[j + 16*i])) {
	        printf("%4c", tas[j + 16*i]);
	    } else {
		printf("    ");
	    }
        }
        printf("\n\n");
    }
    printf("---------------------------------------------------------------\n\n");
}

void tas_init(){
  libre = 0;
  tas[0] = TAILTAS;
  tas[1] = -1;
}

int first_fit(int taille, int *pred) {
    int pos = libre; // début de la zone libre courante
    int taille_courante; // taille de la zone libre courante
    int pred_courant = -1; // adresse du début de la zone précédant la zone courante

    while (pos != -1) {
        // récupération de la taille de la zone libre courante
        taille_courante = tas[pos];

        // si la zone courante est suffisamment grande
        if (taille_courante >= taille) {
            // on la retourne
            *pred = pred_courant;
            return pos;
        }

        // sinon, on passe à la zone suivante
        pred_courant = pos;
        pos = tas[pos + 1];
    }

    // aucune zone libre de taille suffisante n'a été trouvée
    *pred = pred_courant;
    return -1;
}

char* tas_malloc(unsigned int taille) {
  int i = first_fit(taille, &libre);

  libre = i+taille+1;
  tas[libre] = tas[i]-taille-1;
  tas[libre + 1] = -1;
  tas[i] = taille;
  tas[i+1] = libre;

  return &tas[i+1];
}

int tas_free(char *ptr) {
  int index = ptr - (char*)tas - 1; // calcul de l'index de début de la zone
  if (index < 2 || index > TAILTAS-2) {
    // la zone à libérer est invalide
    return -1;
  }
  int prev = -1; // index de début de la zone précédente
  int curr = libre; // index de début de la zone courante
  while (curr != -1 && curr < index) {
    prev = curr;
    curr = tas[curr + 1];
  }
  if (curr != index) {
    // la zone à libérer n'a pas été trouvée
    return -1;
  }
  // la zone à libérer est trouvée, il faut la marquer comme libre
  tas[index-1] = tas[index-2]; // TL devient TD
  tas[index] = prev; // chaînage avec la zone précédente
  if (index + tas[index-1]+1 == curr) {
    // fusion avec la zone libre suivante
    tas[index-1] += tas[curr-1]+1;
    tas[index] = tas[curr];
  }
  if (prev >= 2 && prev + tas[prev-1]+1 == index) {
    // fusion avec la zone libre précédente
    tas[prev-1] += tas[index-1]+1;
    tas[prev+1] = tas[index];
  } else {
    // pas de fusion avec la zone précédente
    tas[prev+1] = index;
  }
  return 0;
}
