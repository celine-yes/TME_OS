
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>

#define MAXFILS 2

//Q2 et Q3
int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Appelez la fonction de la manière suivante:\n");
    printf("%s chaine fichier1 fichier2 ...\n", argv[0]);
    exit(1);
  }

  struct rusage r;

  char *chaine = argv[1];
  int nb_fichiers = argc - 2;
  int nb_fils = 0;  //nb de fils en cours d'exécution

  for (int i = 0; i < MAXFILS && i < nb_fichiers; i++) {
    char *fichier = argv[i + 2];
    pid_t pid = fork();
    if (pid < 0) {
      printf("Erreur lors de la création du processus fils\n");
      continue;
    } else if (pid == 0) {
      // fils
      //printf("Un fils est en cours d'exécution\n");

      execlp("grep", "grep", chaine, fichier, NULL);
      printf("Erreur lors de l'exécution grep sur le fichier %s\n", fichier);
      exit(1);
    } else{
      //pere a bien engendré le fils sans problème donc on incrémente nb_fils
      nb_fils++;
    }
  }

  int fichier_suiv = MAXFILS;
  while (nb_fils > 0){
    pid_t pid = wait3(NULL, 0, &r);
    if(pid == -1){
      printf("Erreur lors de l'attente d'un processus fils\n");
      continue;
    }
    else{
      nb_fils--;
      //printf("Un fils a fini son exécution\n");
    }

    //affichage des temps cpu
    printf("---------------------------------------\nProcessus fils %d:\n", pid);
    printf("temps CPU utilisateur: %f\n", (r.ru_utime.tv_sec * 1000000.0));
    printf("temps CPU systeme: %f\n---------------------------------------\n", (r.ru_stime.tv_sec * 1000000.0));

    if (fichier_suiv < nb_fichiers && nb_fils < nb_fichiers){
      pid_t pid = fork();
      if (pid < 0) {
        printf("Erreur lors de la création du processus fils\n");
        continue;
      }
      else if (pid == 0) {
      // fils
        //printf("Un fils est en cours d'exécution\n");
        char *fichier = argv[fichier_suiv + 2];

        execlp("grep", "grep", chaine, fichier, NULL);
        printf("Erreur lors de l'exécution grep sur le fichier %s\n", fichier);
        exit(1);
      }
      else{
        //pere a bien engendré le fils sans problème donc on incrémente nb_fils
        nb_fils++;
        fichier_suiv++;
        }
    }
  }

  return 0;
}



/*
//Q1
int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Appelez la fonction de la manière suivante:\n");
    printf("%s chaine fichier1 fichier2 ...\n", argv[0]);
    exit(1);
  }

  char *chaine = argv[1];
  int nb_fichiers = argc - 2;

  for (int i = 0; i < nb_fichiers; i++) {
    char *fichier = argv[i + 2];
    pid_t pid = fork();
    if (pid < 0) {
      printf("Erreur lors de la création du processus fils\n");
      continue;
    }
    else if (pid == 0) {
      // fils
      execlp("grep", "grep", chaine, fichier, NULL);
      printf("Erreur lors de l'exécution grep sur le fichier %s\n", fichier);
      exit(1);
    }
    else {
      // père
      int status;
      waitpid(pid, &status, 0);
      if (WIFEXITED(status) && WEXITSTATUS(status) == 1) {
        printf("La chaine %s ne se trouve pas dans le fichier %s\n", chaine, fichier);
      }
    }
  }

  return 0;
}
*/
