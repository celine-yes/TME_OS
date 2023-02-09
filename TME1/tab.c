#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "tab.h"

void InitTab(int *tab, int size){
    srand(time(NULL));
    for( int i=0; i<size; i++){
        tab[i] = rand()%10;
    }
}

void PrintTab(int *tab, int size){
    printf("[%d", tab[0]);
    for(int i=1; i<size; i++){
        printf(", %d", tab[i]);
    }
    printf("]\n");
}

int SumTab(int *tab, int size){
    int somme = tab[0];
    for(int i = 1; i<size; i++){
        somme += tab[i];
    }
    return somme;
}

int MinSumTab(int *min, int *tab, int size){
    *min = tab[0];
    int somme = SumTab(tab, size);

    for(int i = 1; i<size; i++){
        if(*min>tab[i]){
            *min = tab[i];
        }
    }
    return somme;
}

