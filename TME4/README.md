# TME 4

## 1. MULTI-GREP SIMPLE

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
            } else if (pid == 0) {
            // fils
            execlp("grep", "grep", chaine, fichier, NULL);
            printf("Erreur lors de l'exécution grep sur le fichier %s\n", fichier);
            exit(1);
            } else {
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


## 2. MULTI-GREP A PARALLELISME CONTRAINT

    int main(int argc, char **argv) {
      if (argc < 3) {
        printf("Appelez la fonction de la manière suivante:\n");
        printf("%s chaine fichier1 fichier2 ...\n", argv[0]);
        exit(1);
      }

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
          printf("Un fils est en cours d'exécution\n");
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
        int status;
        pid_t pid = wait(&status);
        if(pid == -1){
          printf("Erreur lors de l'attente d'un processus fils\n");
          continue;
        }
        else{
          nb_fils--;
          printf("Un fils a fini son exécution\n");
        }
        if (fichier_suiv < nb_fichiers && nb_fils < nb_fichiers){
          pid_t pid = fork();
          if (pid < 0) {
            printf("Erreur lors de la création du processus fils\n");
            continue;
          }
          else if (pid == 0) {
          // fils
            printf("Un fils est en cours d'exécution\n");
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

En appelant le programme avec ./mgrep "were" "fichier1.txt" "fichier2.txt" "fichier3.txt"

On a pour résultat avec MAXFILS = 2:  

    Un fils est en cours d'exécution
    Un fils est en cours d'exécution
    Gummy bears were originally called "dancing bears."
    Un fils a fini son exécution
    Un fils a fini son exécution
    Un fils est en cours d'exécution
    Un fils a fini son exécution


## 3. MULTI-GREP AVEC STATISTIQUES

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

## 4. PROCESSUS "ZOMBIE"

#### Q4.1

Un processus zombie est l'état d'un processus terminé tant que son père n'a pas pris connaissance de sa terminaison (wait)

#### Q4.2

Voici un programme qui crée pendant 10 secondes deux processus zombie:

    int main(){
      printf("Père commence\n");
      pid_t p1;
      p1 = fork();

      if(p1==0){
        //fils 1
        printf("Fils1 commence\n");
        pid_t p2;
        p2 = fork();
        if(p2==0){
          //fils 2
          printf("Fils2 commence\n");
          printf("Fils2 finit\n");
          exit(0);
        }
        else{
          //fils 1
          printf("Fils1 finit\n");
          exit(0);
        }
      }
      else{
        //père
        sleep(10);
        wait(NULL);
      }
      printf("Père finit\n");
      return 0;
    }
