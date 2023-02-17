
# TME 3

## 1. TESTER LA BIBLIOTHEQUE

### 1.2

    $ cd TME3
    $ make main
    $ ./main

produit l'affichage ci-dessous dans le terminal:

    RANDOM Election !
    Proc. Court1 - 0
    Proc. Court1 - 10000000
    Proc. Court1 - 20000000
    Proc. Court1 - 30000000
    Proc. Court1 - 40000000
    Proc. Court1 - 50000000
    Proc. Court1 - 60000000
    Proc. Court1 - 70000000
    Proc. Court1 - 80000000
    Proc. Court1 - 90000000
    ############ FIN COURT 1

    RANDOM Election !
    Proc. Court0 - 0
    Proc. Court0 - 10000000
    Proc. Court0 - 20000000
    Proc. Court0 - 30000000
    Proc. Court0 - 40000000
    Proc. Court0 - 50000000
    Proc. Court0 - 60000000
    Proc. Court0 - 70000000
    Proc. Court0 - 80000000
    Proc. Court0 - 90000000
    ############ FIN COURT 0

    RANDOM Election !
    Proc. Long0 - 0
    Proc. Long0 - 40000000
    Proc. Long0 - 80000000
    Proc. Long0 - 120000000
    Proc. Long0 - 160000000
    RANDOM Election !
    Proc. Long1 - 0
    Proc. Long1 - 40000000
    Proc. Long1 - 80000000
    Proc. Long1 - 120000000
    Proc. Long1 - 160000000
    RANDOM Election !
    Proc. Long1 - 200000000
    Proc. Long1 - 240000000
    Proc. Long1 - 280000000
    Proc. Long1 - 320000000
    RANDOM Election !
    Proc. Long1 - 360000000
    ############ FIN LONG 1

    RANDOM Election !
    Proc. Long0 - 200000000
    Proc. Long0 - 240000000
    Proc. Long0 - 280000000
    Proc. Long0 - 320000000
    RANDOM Election !
    Proc. Long0 - 360000000
    ############ FIN LONG 0

    PID	FUNCTION	REAL-TIME	CPU-TIME	WAITING-TIME
    --------------------------------------------------------------------
    0	Function0	1.964s		0.747s		1.216s
    1	Function1	1.516s		0.745s		0.772s
    2	Function2	0.472s		0.190s		0.282s
    3	Function3	0.282s		0.282s		0.000s
    --------------------------------------------------------------------
    Average:		1.058s		0.491s		0.567s
    --------------------------------------------------------------------

### 1.3

En diminuant le quantum, cela signifie que les processus seront soumis à des interruptions plus fréquentes pour laisser la possibilité à d'autres processus d'être exécutés.

Ainsi, si on réduit le quantum, les processus seront soumis à des interruptions plus fréquentes, ce qui aura pour effet de réduire le temps total d'exécution de chaque processus. Cependant, cela augmentera également le nombre d'interruptions et la complexité de l'ordonnancement, ce qui peut entraîner une réduction de la performance globale.

En effet en diminuant le quantum à 0.003, on remarque que le temps d'exécution a bien augmenté:

    PID	FUNCTION	REAL-TIME	CPU-TIME	WAITING-TIME
    --------------------------------------------------------------------
    0	Function0	2.055s		0.781s		1.275s
    1	Function1	2.056s		0.796s		1.260s
    2	Function2	0.886s		0.220s		0.666s
    3	Function3	0.762s		0.225s		0.537s
    --------------------------------------------------------------------
    Average:		1.440s		0.506s		0.934s
    --------------------------------------------------------------------


  Et en augmentant le quantum, le processus sera soumis à moins d'interruptions ce qui aura pour effet d'augmenter le temps total d'exécution de chaque processus. Ce qui peut entraîner une augmentation de la performance globale.

En effet en augmentant le quantum à 300 (très excessif), on remarque que le temps d'exécution a bien diminué (de pas beaucoup car on travaille sur très peu de processus):

    PID	FUNCTION	REAL-TIME	CPU-TIME	WAITING-TIME
    --------------------------------------------------------------------
    0	Function0	1.230s		0.758s		0.473s
    1	Function1	1.983s		0.752s		1.230s
    2	Function2	0.473s		0.189s		0.283s
    3	Function3	0.283s		0.283s		0.000s
    --------------------------------------------------------------------
    Average:		0.992s		0.496s		0.497s
    --------------------------------------------------------------------  


## 2. ECRITURE D'UN NOUVEL ALGORITHME D'ORDONNANCEMENT SJF

### 2.1

Il suffit de parcourir la table de processus et trouver le processus avec le plus petit temps d'exécution.

    int SJFElect(void) {
      int p=0;
      double min_time;

      //on cherche le premier processus prêt pour initialiser min_time
      for(int i=0; i<MAXPROC; i++){
        if(Tproc[i].flag == RUN){
          min_time = Tproc[i].duration;
          p=i;
          break;
        }
      }

      printf("SJF Election !\n");

      for(int i=0; i < MAXPROC; i++){
        //!!vérifie si Tproc[i] est prête à être exécuter
        if (Tproc[i].flag == RUN && Tproc[i].duration < min_time){
          p = i;
          min_time = Tproc[i].duration;
        }
      }
      return p;
    }

### 2.2

Après compilation le programme affiche :

    PID	FUNCTION	REAL-TIME	CPU-TIME	WAITING-TIME
    --------------------------------------------------------------------
    0	Function0	3.651s		2.488s		1.163s
    1	Function1	7.235s		2.769s		4.466s
    2	Function2	0.191s		0.191s		0.000s
    3	Function3	0.376s		0.184s		0.191s
    4	Function4	0.490s		0.190s		0.300s
    5	Function5	0.481s		0.199s		0.282s
    6	Function6	0.475s		0.198s		0.276s
    7	Function7	0.466s		0.199s		0.266s
    8	Function8	0.501s		0.201s		0.300s
    9	Function9	0.456s		0.201s		0.255s
    10	Function10	0.365s		0.203s		0.161s
    11	Function11	0.256s		0.209s		0.046s
    --------------------------------------------------------------------
    Average:		1.245s		0.603s		0.642s
    --------------------------------------------------------------------


## 3.APPROXIMATION DE SJF EN TEMPS PARTAGE

### 3.1

    int ApproxSJF(void) {
      int p;
      int elu = GetElecProc();
      int prio_max;

      //on traite le cas ou un processus a été élu
      if(elu != -1){
        //baisser sa priorité en soustrayant le nb de quantums de tmp consommé
        Tproc[elu].prio -= Tproc[elu].ncpu;

        //si prio<0 on le réinitialise a MINPRIO ce qui permet de réduire priorité
        //des processus qui ont consommé plus que les autres
        if(Tproc[elu].prio < 0){
          Tproc[elu].prio = MINPRIO;
        }
      }

      //on cherche le premier processus prêt pour initialiser prio_max
      for(int i=0; i<MAXPROC; i++){
        if(Tproc[i].flag == RUN){
          prio_max = Tproc[i].prio;
          p=i;
          break;
        }
      }

      printf("ApproxSJF Election !\n");

      for (int i = 0; i < MAXPROC; i++) {
        if (Tproc[i].flag == RUN && Tproc[i].prio > prio_max) {
          p = i;
          prio_max = Tproc[i].prio;
        }
      }
      return p;
    }

### 3.2

Pour un quantum d'1 seconde, on a pour résultat avec ApproxSJF:

    PID	FUNCTION	REAL-TIME	CPU-TIME	WAITING-TIME
    --------------------------------------------------------------------
    0	Function0	5.873s		2.123s		3.750s
    1	Function1	5.961s		2.088s		3.873s
    2	Function2	2.174s		0.174s		2.000s
    3	Function3	2.347s		0.173s		2.174s
    4	Function4	2.521s		0.174s		2.347s
    5	Function5	2.164s		0.174s		1.990s
    6	Function6	1.869s		0.174s		1.695s
    7	Function7	1.526s		0.177s		1.349s
    8	Function8	1.109s		0.178s		0.931s
    9	Function9	0.759s		0.174s		0.585s
    10	Function10	1.131s		0.175s		0.956s
    11	Function11	0.787s		0.176s		0.610s
    --------------------------------------------------------------------
    Average:		2.352s		0.497s		1.855s
    --------------------------------------------------------------------

Et avec RandomElect:

    PID	FUNCTION	REAL-TIME	CPU-TIME	WAITING-TIME
    --------------------------------------------------------------------
    0	Function0	5.774s		2.233s		3.541s
    1	Function1	4.016s		2.129s		1.886s
    2	Function2	0.357s		0.175s		0.182s
    3	Function3	0.182s		0.182s		0.000s
    4	Function4	1.177s		0.177s		1.000s
    5	Function5	0.756s		0.174s		0.581s
    6	Function6	2.177s		0.177s		2.000s
    7	Function7	3.309s		0.176s		3.133s
    8	Function8	3.182s		0.175s		3.006s
    9	Function9	1.899s		0.174s		1.725s
    10	Function10	1.199s		0.175s		1.024s
    11	Function11	1.433s		0.175s		1.259s
    --------------------------------------------------------------------
    Average:		2.122s		0.510s		1.611s
    --------------------------------------------------------------------

  On peut remarquer que RandomElect est plus rapide que ApproxSJF mais ApproxSJF.

### 3.3
Dans notre cas, il ne crée pas de famine mais ce n'est pas impossible.
