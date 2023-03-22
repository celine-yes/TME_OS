/* Diffusion tampon 1 case */

  #include <stdio.h>
  #include <unistd.h>
  #include <stdlib.h>
  #include <signal.h>
  #include <libipc.h>

/************************************************************/

/* definition des parametres */

  #define NE          2     /*  Nombre d'emetteurs         */
  #define NR          5     /*  Nombre de recepteurs       */

/************************************************************/

/* definition des semaphores */
  #define SEMNR      0
	#define EMET       1
  #define RECEP      2


/************************************************************/

/* definition de la memoire partagee */

  typedef struct {
    int mess;
    int nb_recepteurs;
  } t_segpart;

  t_segpart *sp;

/************************************************************/

/* variables globales */
  int emet_pid[NE], recep_pid[NR];

/************************************************************/

/* traitement de Ctrl-C */

  void handle_sigint(int sig) {
      int i;
      for (i = 0; i < NE; i++) kill(emet_pid[i], SIGKILL);
      for (i = 0; i < NR; i++) kill(recep_pid[i], SIGKILL);
      det_sem();
      det_shm((char *)sp);
  }

/************************************************************/

/* fonction EMETTEUR */

void emetteur(int mess){
  while(1){
    P(EMET);
    P(SEMNR);
    sp->nb_recepteurs = 0;
    V(SEMNR);
    printf("Emetteur %d a déposé le message %d\n", getpid(), mess);
    sp->mess = mess;

    for(int i=0; i<NR ; i++){
      V(RECEP+i);
    }
  }
}

/************************************************************/

/* fonction RECEPTEUR */

  void recepteur(int id){
  int m;
  while(1){

    P(RECEP + id);
    m = sp->mess;
    printf("Recepteur %d a lu la valeur %d\n", getpid(), m);

    P(SEMNR);
    sp->nb_recepteurs++;
    //printf("Recepteur %d : nb_recepteurs = %d\n", sp->nb_recepteurs);
    V(SEMNR);

    //si tous les récepteurs ont lu les message, on débloque l'émetteur
    if(sp->nb_recepteurs == NR){
      printf("Emetteur libéré\n");
      V(EMET);
    }
  }
  }

/************************************************************/

int main() {
  struct sigaction action;
  /* autres variables (a completer) */
  int semid, pid;
  setbuf(stdout, NULL);

/* Creation du segment de memoire partagee */

  if ( (sp = (t_segpart *)init_shm(sizeof(t_segpart) ))== NULL) {
    perror("init_shm");
    exit(1);
  }

/* creation des semaphores */

  if ((semid = creer_sem(NR + 2)) == -1) {
      perror("creer_sem");
      exit(1);
  }

/* initialisation des semaphores */
  init_un_sem(SEMNR, 1);
  init_un_sem(EMET, 1);
  for (int i = 0; i < NR; i++){
    init_un_sem(RECEP+i, 0);
  }


/* creation des processus emetteurs */
  for (int i = 0; i < NE; i++){
    pid = fork();
    if (pid == -1){
      perror("fork");
      exit(1);
    } else if (pid == 0){
        /* processus fils */
        emetteur(i+1);
        exit(EXIT_SUCCESS);
    } else {
        /* processus pere */
        emet_pid[i] = pid;
    }
  }


/* creation des processus recepteurs */

  for (int i = 0; i < NR; i++){
    pid = fork();
    if (pid == -1){
      perror("fork");
      exit(1);
    } else if (pid == 0){
        /* processus fils */
        recepteur(i);
        exit(EXIT_SUCCESS);
    } else {
        /* processus pere */
        recep_pid[i] = pid;
    }
  }
  for(int i=0; i<NR; i++){
    printf("recep_pid[%d] = %d\n", i, recep_pid[i]);
  }


/* redefinition du traitement de Ctrl-C pour arreter le programme */

    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = handle_sigint;
    sigaction(SIGINT, &action, 0);

    pause();                    /* attente du Ctrl-C  */
    return EXIT_SUCCESS;
}
