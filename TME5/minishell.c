#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/times.h>
#include <time.h>

#define TAILLE 256
#define MAX_ARG 10

int main(){
    char command[TAILLE];
    int attend_fils = 1;
    //pour déterminer tmp d'exécution
    int click_per_sec = sysconf(_SC_CLK_TCK);
    int tmp1, tmp2;
    struct tms cpu1, cpu2;

    printf("Entrez une commande: ");

    while(1){
        fgets(command, TAILLE, stdin);

        // Supprimer le caractère '\n' en fin de chaîne
        command[strcspn(command, "\n")] = 0;
        if (command[strlen(command) - 1] == '&') {
           attend_fils = 0;
           command[strlen(command) - 1] = '\0';
       }

       //quitter le programme si quit
       if (strcmp(command, "quit") == 0) {
            break;
        }

        int i = 0;
        char *arg[MAX_ARG]; //tableau d'arguments
        arg[i] = strtok(command, " "); //récupère la commande à l'indice 0

        //récupère les arguments
        while(arg[i] != NULL && i < MAX_ARG - 1){
          i++;
          arg[i] = strtok(NULL, " ");
        }
        arg[i] = NULL;

        // for(int j=0; j<i; j++){
        //   printf("arg[%d] = %s\n", j, arg[j]);
        // }

        pid_t pid = fork();
        if(pid == 0) {
          //fils
          tmp1 = times(&cpu1);
          execvp(arg[0], arg);
          exit(1);
        }
        else{
          if (attend_fils == 1){

            tmp1 = times(&cpu1);
            int status;
            waitpid(pid, &status, 0);
            tmp2 = times(&cpu2);
            printf("Temps d'exécution de la commande: %.7f\n", (float)(tmp2-tmp1)/click_per_sec);
          }
        }
        printf("Entrez une commande: ");
    }

    return 0;
}
