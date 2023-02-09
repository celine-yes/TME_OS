#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    
    /* Au moins un entier en argument */
    if(argc < 2){
        fprintf(stderr,"Veuillez entrer une liste de nombres entiers SVP !\n");
        fprintf(stderr,"Format : %s n1 [n2 n3 n4 ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int max = atoi(argv[1]);

    for(int i=1; i<argc-1; i++){
        int val = atoi(argv[i]);
        if(val > max){
            max = val;
        }
    }

    printf("Max = %d\n", max);
    return 0;
}