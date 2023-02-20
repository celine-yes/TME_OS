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


