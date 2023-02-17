#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXFILS 5

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s chaine fichier1 fichier2 ... fichierN\n", argv[0]);
        return 1;
    }
    char *chaine = argv[1];
    int nb_fichiers = argc - 2;
    char *fichier;

    // 1ere boucle qui crée les maxfils si besoin
    int i, status, nb_fils = 0; //initialisation de nb_fils
    pid_t pid;
    for (i = 0; i < MAXFILS && i < nbfichiers; i++) {
        pid = fork();
        if (pid == -1) {
            printf("Erreur: impossible de créer un processus fils\n");
            return 1;
        } else if (pid == 0) {
            // Code exécuté par le fils
            filename = argv[i + 2];
            execlp("grep", "grep", chaine, filename, NULL);
            printf("Erreur: impossible d'exécuter grep sur le fichier %s\n", filename);
            return 1;
        } else {
            nb_fils++;
        }
    }

    // Boucle principale pour attendre la fin d'un fils et en créer un nouveau
    int next_file = MAXFILS;
    while (nb_fils > 0) {
        pid = wait(&status);
        if (pid == -1) {
            printf("Erreur: impossible d'attendre un processus fils\n");
            return 1;
        } else {
            nb_fils--;
        }
        if (next_file < num_files) {
            pid = fork();
            if (pid == -1) {
                printf("Erreur: impossible de créer un processus fils\n");
                return 1;
            } else if (pid == 0) {
                // Code exécuté par le fils
                filename = argv[next_file + 1];
                execlp("grep", "grep", chaine, filename, NULL);
                printf("Erreur: impossible d'exécuter grep sur le fichier %s\n", filename);
                return 1;
            } else {
                nb_fils++;
                next_file++;
            }
        }
    }

    return 0;
}
