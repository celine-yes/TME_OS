#include "fat.h"
#include <string.h>
#include <stdio.h>

int file_found (char * file ) {
  int i;
  struct ent_dir * pt = pt_DIR;

  for (i=0; i< NB_DIR; i++) {
    if ((pt->del_flag) && (!strcmp (pt->name, file)))
      return 0;
    pt++;
  }
  /* finchier n'existe pas */
  return 1;
}


void list_fat () {
  int i;
  short *pt = pt_FAT;
  for (i=0; i < NB_ENT_FAT; i++) {
    if (*pt)
      printf ("%d ",i);
    pt++;
  }
  printf ("\n");
}


void list_dir ( ) {
  /* A COMPLETER */
  int i, count = 0;
  struct ent_dir * pt = pt_DIR;
  printf("Nom du fichier\t Taille\t\t Blocs de données\n");
  for (i=0; i< NB_DIR; i++) {
      if (pt->del_flag) {
          printf("%s\t", pt->name);
          printf("%d octets\t\t", pt->size);
          short bloc = pt->first_bloc;
          while (bloc != FIN_FICHIER) {
              printf("%d ", bloc);
              bloc = *((pt_FAT) + bloc);
          }
          printf("\n");
          count++;
      }
      pt++;
  }
  printf("\nNombre de fichiers : %d\n", count);
}

int cat_file (char* file) {
 /* A COMPLETER */
  int i, j, trouve = 0;
  struct ent_dir *pt = pt_DIR;
  short *fat_entry;
  char buffer[SIZE_SECTOR];

  // Cherche le fichier file dans le répertoire
  for (i = 0; i < NB_DIR; i++) {
      if ((pt->del_flag) && (!strcmp(pt->name, file))) {
          trouve = 1;
          // Lit et affiche le contenu de chaque bloc du fichier
          fat_entry = pt_FAT + pt->first_bloc;
          while (*fat_entry != FIN_FICHIER) {
              if (read_sector(*fat_entry, buffer) == -1) {
                  printf("Erreur de lecture.\n");
                  return -1;
              }
              for (j = 0; j < SIZE_SECTOR; j++) {
                  putchar(buffer[j]);
              }
              fat_entry = pt_FAT + *fat_entry;
          }
          // Affiche le contenu du dernier bloc
          if (read_sector(*fat_entry, buffer) == -1) {
              printf("Erreur de lecture.\n");
              return -1;
          }
          for (j = 0; j < pt->size % SIZE_SECTOR; j++) {
              putchar(buffer[j]);
          }
          printf("\n");
          return 0;
      }
      pt++;
  }
  // Le fichier file n'a pas été trouvé
  return -1;
}

int mv_file (char*file1, char *file2) {
   /* A COMPLETER */
  int i;
  struct ent_dir *pt = pt_DIR;
  // Chercher le fichier file1 dans le répertoire
  for (i = 0; i < NB_DIR; i++) {
      if ((pt->del_flag) && (!strcmp(pt->name, file1))) {
          // Mettre à jour le nom du fichier dans l'entrée du répertoire
          strcpy(pt->name, file2);
          // Écrire le répertoire et la FAT sur le disque et si erreur renvoie -1
          if (write_DIR_FAT_sectors() == -1) {
              printf("Erreur d'écriture sur disque.\n");
              return -1;
          }
          return 0;
      }
      pt++;
  }
  // Le fichier file1 n'a pas été trouvé
  return -1;
}


int delete_file(char *file) {
  struct ent_dir *pt = pt_DIR;
  int i, j;
  short bloc_courant;
  for (i = 0; i < NB_DIR; i++) {
    if ((pt->del_flag) && (!strcmp(pt->name, file))) { // Fichier trouvé
      // Supprime le fichier du répertoire
      pt->del_flag = 0;
      // Libération des blocs occupés par le fichier
      bloc_courant = pt->first_bloc;
      while (bloc_courant != FIN_FICHIER && bloc_courant != 0) {
        /* On marque le bloc courant comme libre */
        pt_FAT[bloc_courant] = 0;
        // Passe au bloc suivant
        bloc_courant = pt_FAT[bloc_courant];
      }
      // Sauvegarde le répertorire et la FAT sur le disque
      if (write_DIR_FAT_sectors() == -1) {
        fprintf(stderr, "Erreur lors de l'écriture du répertoire et de la FAT sur disque.\n");
        return -1;
      }
      return 0;
    }
    pt++;
  }
  // fichier n'existe pas
  return -1;
}

int create_file (char *file) {
  /* A COMPLETER */
  struct ent_dir *pt = pt_DIR;
  int i, j;
  short first_free_bloc, num_new_file;

  // Vérifie si le fichier existe déjà
  for (i = 0; i < NB_DIR; i++) {
      if ((pt->del_flag) && (!strcmp(pt->name, file))) {
          return -1;
      }
      pt++;
  }

  // Recherche une entrée libre dans le répertoire
  pt = pt_DIR;
  for (i = 0; i < NB_DIR; i++) {
      if (pt->del_flag == 0) {
          // Initialise la nouvelle entrée du répertoire
          num_new_file = i;
          pt->del_flag = 1;
          strcpy(pt->name, file);
          pt->size = 0;
          pt->first_bloc = FIN_FICHIER;
          pt->last_bloc = FIN_FICHIER;

          // Recherche un bloc libre dans la FAT
          first_free_bloc = 0;
          for (j = 0; j < NB_ENT_FAT; j++) {
              if (pt_FAT[j] == 0) {
                  first_free_bloc = j;
                  pt_FAT[first_free_bloc] = FIN_FICHIER;
                  break;
              }
          }
          if (first_free_bloc == 0) {
              printf("Impossible de créer le fichier %s: disque plein.\n", file);
              return -1;
          }

          // Met à jour la FAT et le répertoire sur le disque
          if (write_DIR_FAT_sectors() == -1) {
              printf("Erreur lors de l'écriture du répertoire et de la FAT sur disque.\n");
              return -1;
          }
          return 0;
      }
      pt++;
  }

  // Pas d'entrée libre dans le répertoire
  return -1;
}


short alloc_bloc () {
  /* A COMPLETER */
  int i;
  // Parcourt la FAT pour trouver un bloc libre
  for (i = 0; i < NB_ENT_FAT; i++) {
    if (pt_FAT[i] == 0) {
      // Bloc libre trouvé, on l'alloue
      pt_FAT[i] = FIN_FICHIER;
      // Sauvegarde la FAT sur disque
      if (write_sector(NB_SECTOR_FAT + i / NB_FAT_ENT_PER_SECTOR, (char*)pt_FAT + (i % NB_FAT_ENT_PER_SECTOR) * sizeof(short)) == -1) {
        fprintf(stderr, "Erreur lors de l'écriture de la FAT sur disque.\n");
        return -1;
      }
      return i;
    }
  }
  // Pas de bloc libre
  return -1;
}

int append_file  (char*file, char *buffer, short size) {
  /* A COMPLETER */
  struct ent_dir *pt = pt_DIR;
  int i;
  short bloc_courant;
  // Recherche du fichier dans le répertoire
  for (i = 0; i < NB_DIR; i++) {
    if ((pt->del_flag) && (!strcmp(pt->name, file))) {
      // Fichier trouvé
      // Alloue un nouveau bloc pour la fin du fichier
      short new_bloc = alloc_bloc();
      if (new_bloc == -1) {
        return -1;
      }
      // Ajoute le nouveau bloc à la fin de la chaîne de blocs
      bloc_courant = pt->first_bloc;
      while (pt_FAT[bloc_courant] != FIN_FICHIER) {
        bloc_courant = pt_FAT[bloc_courant];
      }
      pt_FAT[bloc_courant] = new_bloc;
      pt_FAT[new_bloc] = FIN_FICHIER;
      // Écrit le contenu du tampon buffer sur le nouveau bloc
      if (write_sector(new_bloc * SIZE_SECTOR / sizeof(short), buffer) == -1) {
        printf("Erreur lors de l'écriture du nouveau bloc.\n");
        return -1;
      }
      // Met à jour la taille du fichier
      pt->size += size;
      // Sauvegarde le répertoire et la FAT sur disque
      if (write_DIR_FAT_sectors() == -1) {
        printf("Erreur lors de l'écriture du répertoire et de la FAT sur disque.\n");
        return -1;
      }
      return 0;
    }
    pt++;
  }
  // fichier n'existe pas
  return -1;
}

struct ent_dir*  read_dir (struct ent_dir *pt_ent ) {
  /* A COMPLETER */
}
