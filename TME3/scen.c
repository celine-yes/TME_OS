#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include "libsched/src/sched.h"

#define LONGTIME 8E8
void ProcLong(int *);
void ProcCourt(int *);

// Exemple de processus long (une simple bouble),
// Chaque processus long cr�e a son tour 4 processus courts
//
void ProcLong(int *pid) {
  long i;
  static int cpt = 0;

  for (i=0;i<LONGTIME;i++) {
    if (i%(long)(LONGTIME/4) == 0)  {
      int *tcpt = (int *) malloc(sizeof(int));
      *tcpt = cpt;
      CreateProc((function_t)ProcCourt,(void *)tcpt, 10);
      cpt++;
    }
    if (i%(long)(LONGTIME/100) == 0)
      printf("Proc. Long %d - %ld\n",*pid, i);
  }
  printf("############ FIN LONG %d\n\n", *pid );
}


// Processus court
void ProcCourt(int *pid) {
  long i;

  for (i=0;i<LONGTIME/10;i++)
    if (i%(long)(LONGTIME/100) == 0)
      printf("Proc. Court %d - %ld\n",*pid, i);
  printf("############ FIN COURT %d\n\n", *pid );
}




// Exemples de primitive d'election definie par l'utilisateur
// Remarques : les primitives d'election sont appel�es directement
//             depuis la librairie. Elles ne sont app�l�es que si au
//             moins un processus est � l'etat pret (RUN)
//             Ces primitives manipulent la table globale des processus
//             d�finie dans sched.h


// Election al�atoire
int RandomElect(void) {
  int i;

  printf("RANDOM Election !\n");

  do {
    i = (int) ((float)MAXPROC*rand()/(RAND_MAX+1.0));
  } while (Tproc[i].flag != RUN);

  return i;
}


// Election de SJF "Shortest Job Fisrt"
int SJFElect(void) {
  int p=0;
  double min_time;
  int i = 0;

  //on cherche le premier processus prêt pour soutirer sa durée
  do{
    if(Tproc[i].flag == RUN){
      min_time = Tproc[i].duration;
    }
    i++;
  }while(i<MAXPROC);

  printf("SJF Election !\n");

  for(int i=0; i < MAXPROC; i++){
    double tmp = Tproc[i].duration;
    //printf("Tproc[%d].flag = %d\n", i, Tproc[i].flag);

    //!!vérifie si Tproc[i] est prête à être exécuter
    if (Tproc[i].flag == RUN && (tmp < min_time)){
      printf("tmp = %f p=%d------------------------------\n", tmp, i);
      p = i;
      min_time = tmp;
    }
  }
  return p;
}

// Approximation SJF
int ApproxSJF(void) {
  int p;

  /* Choisir le processus p - A ecrire en TP */

  return p;
}



int main (int argc, char *argv[]) {
  int i;
  int *j;

  // Cr�er les processus long
  for  (i = 0; i < 2; i++) {
    j = (int *) malloc(sizeof(int));
    *j= i;
    CreateProc((function_t)ProcLong,(void *)j, 80);
  }

  // // Cr�er les processus court
  // for  (i = 0; i < 2; i++) {
  //   j = (int *) malloc(sizeof(int));
  //   *j= i;
  //   CreateProc((function_t)ProcCourt,(void *)j, 10);
  // }

  // Definir une nouvelle primitive d'election avec un quantum de 0.5 seconde
  SchedParam(NEW, 0.3, SJFElect);

  // Lancer l'ordonnanceur en mode non "verbeux"
  sched(0);

  // Imprimer les statistiques
  PrintStat();

  return EXIT_SUCCESS;

}
