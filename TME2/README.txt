
EXERCICE 1

1.1
En exécutant la commande time sleep 5, on a pour résultat:
real	0m5,002s
user	0m0,001s
sys	0m0,001s
le temps réel étant de 5 secondes

1.2
En exécutant la commande time ./loopcpu, on a pour résultat:
real	0m15,225s
user	0m15,180s
sys	0m0,004s
le temps réel étant de 15 secondes dont 15 secondes en mode user

1.3
En exécutant la commande time ./loopsys, on a pour résultat:
real	0m8,891s
user	0m3,019s
sys	0m5,842s
le temps réel étant de 8 secondes dont 3 secondes en mode user et 5 secondes en mode system

EXERCICE 3

3.2
En exécutant la commande time ./mytimes "sleep 5" "sleep 10", on a pour résultat:
Temps mis par l'exécution de la commande sleep 5: 5.00247717
Temps mis par l'exécution de la commande sleep 10: 10.00281715

EXERCICE 4

4.2
En exécutant la commande time ./mytimes "sleep 5" ./loopcpu ./loopsys, on a pour résultat:

Statistiques de 'sleep 5':
Temps total: 5.010000
Temps utilisateur: 0.000000
Temps systeme: 0.000000
Temps utilisateur fils: 0.000000
Temps systeme fils: 0.000000

Statistiques de './loopcpu':
Temps total: 9.690000
Temps utilisateur: 0.000000
Temps systeme: 0.000000
Temps utilisateur fils: 9.660000
Temps systeme fils: 0.000000

Statistiques de './loopsys':
Temps total: 5.640000
Temps utilisateur: 0.000000
Temps systeme: 0.000000
Temps utilisateur fils: 1.990000
Temps systeme fils: 3.640000

EXERCICE 5

5.1
La priorité du processus a une valeur de 80

5.2
Après avoir tapé la commande nice -19 ps -l
la priorité du processus a désormais une valeur de 99.

5.3
Après avoir tapé la commande suivante:
./mytimes ./loopcpu & ./mytimes ./loopcpu & nice -19 ./mytimes ./loopcpu &
./mytimes ./loopcpu & ./mytimes ./loopcpu & ./mytimes ./loopcpu & ./mytimes ./loopcpu &
 ./mytimes ./loopcpu & ./mytimes ./loopcpu&

On obtient en résultat:

Temps total: 11.570000
Temps utilisateur: 0.000000
Temps systeme: 0.000000
Temps utilisateur fils: 10.890000
Temps systeme fils: 0.010000

Statistiques de './loopcpu':
Temps total: 11.800000
Temps utilisateur: 0.000000
Temps systeme: 0.000000
Temps utilisateur fils: 10.870000
Temps systeme fils: 0.000000

Statistiques de './loopcpu':
Temps total: 11.800000
Temps utilisateur: 0.000000
Temps systeme: 0.000000
Temps utilisateur fils: 11.080000
Temps systeme fils: 0.010000

Statistiques de './loopcpu':
Temps total: 12.030000
Temps utilisateur: 0.000000
Temps systeme: 0.000000
Temps utilisateur fils: 10.950000
Temps systeme fils: 0.010000

Statistiques de './loopcpu':
Temps total: 12.280000
Temps utilisateur: 0.000000
Temps systeme: 0.000000
Temps utilisateur fils: 11.000000
Temps systeme fils: 0.040000

Statistiques de './loopcpu':
Temps total: 12.290000
Temps utilisateur: 0.000000
Temps systeme: 0.000000
Temps utilisateur fils: 10.940000
Temps systeme fils: 0.010000

Statistiques de './loopcpu':
Temps total: 12.410000
Temps utilisateur: 0.000000
Temps systeme: 0.000000
Temps utilisateur fils: 10.960000
Temps systeme fils: 0.040000

Statistiques de './loopcpu':
Temps total: 12.420000
Temps utilisateur: 0.000000
Temps systeme: 0.000000
Temps utilisateur fils: 10.940000
Temps systeme fils: 0.020000

Statistiques de './loopcpu':
Temps total: 20.840000
Temps utilisateur: 0.000000
Temps systeme: 0.000000
Temps utilisateur fils: 9.820000
Temps systeme fils: 0.000000
