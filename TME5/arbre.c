#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char ** argv){
    if (argc != 2) {
        printf("Appelez la fonction de la manière suivante:\n");
        printf("%s <hauteur_arbre>\n", argv[0]);
        exit(1);
    }
    printf("%d\n", getpid());

    int hauteur = atoi(argv[1]);
    for(int i=0; i<hauteur; i++){
        if(fork() != 0){
            int p = fork();
            wait(NULL);
        }
    }

    sleep(30);
    return 0;
}

