#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#include <time.h>

/*
float time_diff(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec);
}

void lance_commande(char* commande){
  //Q3.1
  struct timeval tmp_debut;
  struct timeval tmp_fin;

  gettimeofday(&tmp_debut,NULL);
  int err = system(commande);
  gettimeofday(&tmp_fin,NULL);

  if(err==-1){
    printf("Erreur lors de l'exécution de la commande %s\n", commande);
  }
  else{
    printf("Temps mis par l'exécution de la commande %s: %.8f\n", commande, time_diff(&tmp_debut, &tmp_fin));
  }
}
*/

void lance_commande(char* commande){
  //Q4.1
  int tmp1, tmp2;
  struct tms cpu1, cpu2;
  int click_per_sec = sysconf (_SC_CLK_TCK);

  tmp1 = times(&cpu1);
  int err = system(commande);
  tmp2 = times(&cpu2);

  if(err==-1){
    printf("Erreur lors de l'exécution de la commande %s\n", commande);
  }
  else{
    printf("Statistiques de '%s': \n", commande);
    printf("Temps total: %.6f\n", (float)(tmp2-tmp1)/click_per_sec);
    printf("Temps utilisateur: %.6f\n", (float)(cpu2.tms_utime - cpu1.tms_utime)/click_per_sec);
    printf("Temps systeme: %.6f\n", (float)(cpu2.tms_stime - cpu1.tms_stime)/click_per_sec);
    printf("Temps utilisateur fils: %.6f\n", (float)(cpu2.tms_cutime - cpu1.tms_cutime)/click_per_sec);
    printf("Temps systeme fils: %.6f\n\n", (float)(cpu2.tms_cstime - cpu1.tms_cstime)/click_per_sec);

  }
}

int main(int argc, char** argv){
  if(argc<2){
    printf("Veuillez mettre en arguments des commandes\n");
    printf("Exemple: %s [commande1] [commande2] ...\n", argv[0]);
    return 0;
  }

  for(int i=1; i<argc; i++){
    lance_commande(argv[i]);
  }

  return 0;
}
