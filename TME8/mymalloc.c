#include "affiche_tas.h"

char tas[TAILTAS  +1];
int libre;

/* A COMPLETER */

int main() {
  tas_init();

  char *p1, *p2, *p3, *p4, *p5;
  p1 = (char *) tas_malloc(10);
  p2 = (char *) tas_malloc(9);
  p3 = (char *) tas_malloc(5);
  afficher_tas();
  strcpy( p1, "tp 1" );
  strcpy( p2, "tp 2" );
  strcpy( p3, "tp 3" );
  tas_free( p2 );
  p4 = (char *) tas_malloc(8);
  strcpy( p4, "systeme" );

  afficher_tas();
}
