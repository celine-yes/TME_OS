#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    char command[256];
    while(command){
        scanf("%s", &command);
    }

    printf("sortie de boucle\n");

    return 0;
}