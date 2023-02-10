
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
      double min_time = Tproc[0].duration;

      printf("SJF Election !\n");

      for(int i=0; i < MAXPROC; i++){
        //!!vérifie si Tproc[i] est prête à être exécuter
        if ((Tproc[i].duration < min_time) && Tproc[i].flag != RUN){
          p = i;
          min_time = Tproc[i].duration;
        }
      }

      return p;
    }

### 2.2
