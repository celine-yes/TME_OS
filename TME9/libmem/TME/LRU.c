#include "LRU.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int	initLRU(Swapper*);
void	referenceLRU(Swapper*,unsigned int frame);
unsigned int chooseLRU(Swapper*);
void	finalizeLRU(Swapper*);

typedef struct {
	unsigned int clock;
	unsigned int * age;
} InfoLRU;

int initLRUSwapper(Swapper*swap,unsigned int frames){
 return	initSwapper(swap,frames,initLRU,referenceLRU,chooseLRU,finalizeLRU);
}

int	initLRU(Swapper*swap){
	InfoLRU *info = malloc(sizeof(InfoLRU));
    if (info == NULL) {
        return -1; // erreur d'allocation de mémoire
    }
    info->clock = 0;
    info->age = calloc(swap->frame_nb, sizeof(unsigned int));
    if (info->age == NULL) {
        free(info);
        return -1; // erreur d'allocation de mémoire
    }
    swap->private_data = info;
    return 0;
}

void	referenceLRU(Swapper*swap,unsigned int frame){
	InfoLRU *info = (InfoLRU*)swap->private_data;
  info->age[frame] = info->clock++;
}

unsigned int chooseLRU(Swapper*swap){
	InfoLRU *info = (InfoLRU*)swap->private_data;
    unsigned int lru_frame = 0;
    unsigned int lru_age = info->age[0];
    for (unsigned int i = 1; i < swap->frame_nb; i++) {
        if (info->age[i] < lru_age) {
            lru_age = info->age[i];
            lru_frame = i;
        }
    }
    info->age[lru_frame] = info->clock++;
    return lru_frame;
}

void	finalizeLRU(Swapper*swap){
	InfoLRU *info = (InfoLRU*)swap->private_data;
    free(info->age);
    free(info);

}
