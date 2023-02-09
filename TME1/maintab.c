#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "tab.h"
#include <sys/time.h>
#include <sys/resource.h>

 void PrintMem(){
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    printf("Usage %ld\n", usage.ru_maxrss);
 }

int main(int argc, char *argv[]){
    
    int tab1[NMAX];
    int * tab2 = malloc(sizeof(int)*NMAX);
    assert(tab2);
    int min, somme;

    printf("Avant l'initialisation : ");
    PrintMem();
    InitTab(tab1, NMAX);
    InitTab(tab2, NMAX);
    printf("Après l'initialisation : ");
    PrintMem();

    //PrintTab(tab1, NMAX);
    //PrintTab(tab2, NMAX);

    /*somme = MinSumTab(&min, tab1, NMAX);
    printf("MinSumTab du premier tableau: somme = %d, min = %d\n", somme, min);
    somme = MinSumTab(&min, tab2, NMAX);
    printf("MinSumTab du deuxième tableau: somme = %d, min = %d\n", somme, min);
*/
    free(tab2);

    return 0;
}