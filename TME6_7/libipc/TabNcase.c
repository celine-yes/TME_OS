/* Diffusion tampon N case */

/* Ne marche pas :/ */

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

  #define SEM_EMET      0
  #define SEM_RECEP     1
  int CASE_EMET[NMAX];
  int CASE_RECEP[NMAX];


/************************************************************/

/* definition de la memoire partagee */
  typedef struct {
    int id;
    int ir;
    int nb_caseVide;  // si =0 alors les émetteurs sont bloqués
                      // si =NMAX alors les récepteurs sont bloqués
    int * nb_recepteurs;//nb de recepteurs ayant lu le message dans case i
    int ** lu;  // enregistre les messages lus par chaque récepteur
    int * mess; // tableau qui stocke les messages
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
  free(sp->mess);
  free(sp->nb_recepteurs);

  }

/************************************************************/

/* fonction EMETTEUR */


void emetteur(int ind) {
    int mess = ind;
    int pos;
    while (1) {
        P(SEM_EMET);  // Attend qu'une case soit vide

        pos = sp->id % NMAX;

        P(CASE_EMET[pos]);  // Verrouille la case pour écrire

        sp->id++;
        sp->mess[pos] = mess;          // écriture
        sp->nb_recepteurs[pos] = 0;    // réinitialisation

        printf("Emetteur %d a déposé le message %d dans la case %d\n", getpid(), mess, pos);

        V(CASE_EMET[pos]);  // Déverrouille la case
        for(int i=0; i<NR; i++){
          V(SEM_RECEP);  // Un message est maintenant disponible à lire
        }
    }
}

/******            V(CASE_RECEP[pos]);******************************************************/

/* fonction RECEPTEUR */

void recepteur(int ind) {
    int m;
    int pos;

    while (1) {
        //P(SEM_RECEP);  // Attend qu'un message soit disponible à lire

        pos = sp->ir % NMAX;
        //printf("pos = %d\n", pos);
        P(CASE_RECEP[pos]);  // Verrouille la case pour lire

        // Vérifie si le récepteur a déjà lu le message
        if (sp->lu[ind][pos] == 0) {
            sp->nb_recepteurs[pos]++;
            m = sp->mess[pos];
            sp->lu[ind][pos] = 1; // Marque le message comme lu

            printf("Recepteur %d a lu la valeur %d dans la case %d\n", getpid(), m, pos);

            // Si tous les récepteurs ont lu le message de cette case
            // le dernier qui le lit libère la case pour un émetteur
            if (sp->nb_recepteurs[pos] == NR) {
                sp->ir++;

                //V(SEM_EMET);  // Libère une case vide pour un émetteur
            } else {
                //V(SEM_RECEP);  // Un autre récepteur peut lire ce message
                V(CASE_RECEP[pos]);  // Déverrouille la case
                continue;
            }
        } else {
            // Si le récepteur a déjà lu le message, libère simplement le sémaphore
            //V(SEM_RECEP);
        }

        V(CASE_RECEP[pos]);  // Déverrouille la case
    }
}


/************************************************************/

int main() {
    struct sigaction action;
    /* autres variables (a completer) */
    int semid, pid;
    setbuf(stdout, NULL);

    //suite définition des sémaphores

    for(int i = 0; i < NMAX; i++){
      CASE_EMET[i] = i+2;
    }

    for(int i = 0; i < NMAX; i++){
      CASE_RECEP[i] = i + 2 + NMAX;
    }
/* Creation du segment de memoire partagee */

    if ( (sp = (t_segpart *)init_shm(sizeof(t_segpart) ))== NULL) {
      perror("init_shm");
      exit(1);
    }

    sp->id = 0;
    sp->ir = 0;
    sp->nb_caseVide = NMAX;
    sp->nb_recepteurs = malloc(sizeof(int) * NMAX);
    sp->mess = malloc(sizeof(int) * NMAX);
    sp->lu = malloc(sizeof(int *) * NR);
    for (int i = 0; i < NR; i++) {
        sp->lu[i] = calloc(NMAX, sizeof(int));
    }

/* creation des semaphores */

    if ((semid = creer_sem(NR + NMAX + NE + 2)) == -1) {
        perror("creer_sem");
        exit(1);
    }

/* initialisation des semaphores */

    init_un_sem(SEM_EMET, NMAX);
    init_un_sem(SEM_RECEP, NR);
    for(int i = 0; i < NMAX; i++){
      init_un_sem(CASE_EMET[i], 1);
      init_un_sem(CASE_RECEP[i], NR);
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
