#include "Fifo.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int initFifo(Swapper*);
unsigned int fifoChoose(Swapper*);
void fifoReference(Swapper*,unsigned int frame);
void finalizeFifo(Swapper*);

int initFifoSwapper(Swapper*swap,unsigned int frames){
	return	initSwapper(swap,frames,initFifo,NULL,fifoChoose,finalizeFifo);
}

int initFifo(Swapper*swap){
	/* A ecrire en TME */
	unsigned int last_frame = 0;
	swap->private_data = last_frame;
	return 0;
}

unsigned int fifoChoose(Swapper*swap){
	/* A ecrire en TME */
	unsigned int res = swap->private_data;
	//printf("frame_nb = %d\n", swap->frame_nb);
	//printf("res = %d\n", swap->private_data);
	if(res < swap->frame_nb - 1){
		 *(swap->private_data)++;
	}
	else{
		swap->private_data = 0;
	}
	return res;
}

void finalizeFifo(Swapper*swap){
	free(swap->private_data);
}
