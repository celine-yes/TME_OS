/* Diffusion tampon N case */

  #include <stdio.h>
  #include <unistd.h>
  #include <signal.h>
  #include <libipc.h>

/************************************************************/

/* definition des parametres */

  #define NE          2     /*  Nombre d'emetteurs         */
  #define NR          5     /*  Nombre de recepteurs       */
  #define NMAX        3     /*  Taille du tampon           */

/************************************************************/

/* definition des semaphores */

  #define SEMNR      0
  int EMET[NE];
  int RECEP[NR];
  int CASE[NMAX];


/************************************************************/

/* definition de la memoire partagee */
  typedef struct {
    int id;
    int ir;
    int * mess;
    int * nb_recepteurs;
  } t_segpart;

t_segpart *sp;
/************************************************************/

/* variables globales */
    int emet_pid[NE], recep_pid[NR];

/************************************************************/

/* traitement de Ctrl-C */

  void handle_sigint(int sig)
  { int i;
  	for (i = 0; i < NE; i++) kill(emet_pid[i], SIGKILL);
	for (i = 0; i < NR; i++) kill(recep_pid[i], SIGKILL);
	det_sem();
	det_shm((char *)sp);

  }

/************************************************************/

/* fonction EMETTEUR */


  void emetteur(int ind) {
    int mess = ind;
    while(1){
      int i;
      P(EMET[ind]);
      i = sp->id;
      sp->id = (sp->id + 1) % NMAX;
      P(CASE[i]);

      sp->mess[ind] = mess;
      sp->nb_recepteurs[ind] = 0;
      printf("Emetteur %d a déposé le message %d dans la case %d\n", getpid(), mess, ind);
      V(EMET[ind]);
      V(CASE[ind]);
      for(int i=0; i < NR; i++){
        V(RECEP[i]);
      }
    }
  }

/************************************************************/

/* fonction RECEPTEUR */

  void recepteur(int ind){
    int m;
    while(1){
      int case_id;
      P(RECEP[ind]);
      case_id = sp->ir;
      sp->ir = (sp->ir + 1) % NMAX;
      m = sp->mess[case_id];
      printf("Recepteur %d a lu la valeur %d de la case %d\n", getpid(), m, case_id);

      P(CASE[case_id]);
      P(RECEP[ind]);
      P(SEMNR);
      sp->nb_recepteurs[case_id]++;
      V(SEMNR);

      if(sp->nb_recepteurs[case_id] == NR){

        printf("Emetteur de la case %d libéré\n", case_id);
        V(CASE[case_id]);
      }
    }
  }

/************************************************************/

int main() {
    struct sigaction action;
    /* autres variables (a completer) */
    int semid, pid;
    setbuf(stdout, NULL);

    //suite définition des sémaphores

    for(int i = 0; i < NE; i++){
      EMET[i] = i+1;
    }

    for(int i = 0; i < NR; i++){
      RECEP[i] = i + 1 + NE;
    }

    for(int i = 0; i < NMAX; i++){
      CASE[i] = i + 1 + NE + NR;
    }
/* Creation du segment de memoire partagee */

    if ( (sp = (t_segpart *)init_shm(sizeof(t_segpart) ))== NULL) {
      perror("init_shm");
      exit(1);
    }

    sp->id = 0;
    sp->ir = 0;

/* creation des semaphores */

    if ((semid = creer_sem(NR + NMAX + NE + 1)) == -1) {
        perror("creer_sem");
        exit(1);
    }

/* initialisation des semaphores */

    init_un_sem(SEMNR, 1);
    for(int i = 0; i < NE; i++){
      init_un_sem(EMET[i], NE);
    }
    for(int i = 0; i < NR; i++){
      init_un_sem(RECEP[i], 0);
    }
    for(int i = 0; i < NMAX; i++){
      init_un_sem(CASE[i], NR);
    }

/* creation des processus emetteurs */

    for (int i = 0; i < NE; i++){
      pid = fork();
      if (pid == -1){
        perror("fork");
        exit(1);
      } else if (pid == 0){
          /* processus fils */
          emetteur(i);
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
    // for(int i=0; i<NR; i++){
    //   printf("recep_pid[%d] = %d\n", i, recep_pid[i]);
    // }

/* redefinition du traitement de Ctrl-C pour arreter le programme */

    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = handle_sigint;
    sigaction(SIGINT, &action, 0);

    pause();                     /* attente du Ctrl-C */
    return EXIT_SUCCESS;
}
