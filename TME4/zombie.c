#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
  printf("Père commence\n");
  pid_t p1;
  p1 = fork();

  if(p1==0){
    //fils 1
    printf("Fils1 commence\n");
    pid_t p2;
    p2 = fork();
    if(p2==0){
      //fils 2
      printf("Fils2 commence\n");
      printf("Fils2 finit\n");
      exit(0);
    }
    else{
      //fils 1
      printf("Fils1 finit\n");
      exit(0);
    }
  }
  else{
    //père
    sleep(10);
    wait(NULL);
  }
  printf("Père finit\n");
  return 0;
}
