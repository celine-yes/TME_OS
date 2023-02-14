#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv){

  //vérification arguments appel fonction
  if(argc < 3){
    printf("Veuillez lancer le programme ainsi:\n");
    printf("%s chaine fichier1 fichier2 ...", argv[0]);
    exit(1);
  }

  char * chaine = argv[1];
  int nb_fichiers = argc - 2; //sans compter argv[0] et argv[1]

  for(int i=0; i<nb_fichiers; i++){
    char* fichier = argv[i+2]; //pareil, sans compter argv[0] et argv[1]
    pid_t pid = fork();
    if(pid<0){
      printf("Erreur lors de la création du processus fils\n");
      exit(1);
    }
    if(pid==0){  //Fils
      execlp("grep", "grep", chaine, fichier, NULL);
      printf("Erreur lors de l'exécution de grep sur le fichier %s\n",fichier);
      exit(1);
    }
    else{  //Père
      int status;
      waitpid(pid, &status, 0);
      if(WIFEXITED(status) && WEXITSTATUS(status) == 1){
        printf("processus fils a rencontré des erreur lors de son exécution\n");
      }
    }
  }
  return 0;
}
