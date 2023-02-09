#include <stdio.h>
#include <unistd.h>

int main(){
  for(long i=0; i<50000000; i++){
    getpid();
  }
  return 0;
}
