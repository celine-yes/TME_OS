# TME10-11

## 1. CREATION ENVIRONNEMENT

### 1.1
la FAT est composée de 128 entrées, chacune occupant 2 octets, ce qui donne une taille totale de 256 octets.
Comme chaque entrée de la FAT peut référencer un bloc de 128 octets, le nombre maximum de blocs qu'elle peut référencer est de 256 x 128 / 2 = 16384 blocs.
La taille maximale d'un fichier dans ce système de fichiers est de 16384 x 128 octets, soit 2 097 152 octets (environ 2 mo).

## 2. LISTAGE DU REPERTOIRE

### 2.1

    void list_dir ( ) {
      /* A COMPLETER */
      int i, count = 0;
      struct ent_dir * pt = pt_DIR;
      printf("Nom du fichier\t Taille\t Blocs de données\n");
      for (i=0; i< NB_DIR; i++) {
          if (pt->del_flag) {
              printf("%s\t", pt->name);
              printf("%d octets\n", pt->size);
              count++;
          }
          pt++;
      }
      printf("\nNombre de fichiers : %d\n", count);
    }

### 2.2

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

Résultat:

    Nom du fichier	 Taille		 Blocs de données
    fichier1	512 octets		6 8 9 11
    fichier2	380 octets		7 12 13
    fichier3	252 octets		14 15
    fichier4	378 octets		18 20 16

    Nombre de fichiers : 4


## 3. CHANGEMENT DU NOM D'UN FICHIER

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

  Résultat:

    ./test_mv_file fichier4 fichier6

      REPETOIRE AVANT:
    Nom du fichier	 Taille		 Blocs de données
    fichier1	512 octets		6 8 9 11
    fichier2	380 octets		7 12 13
    fichier3	252 octets		14 15
    fichier4	378 octets		18 20 16

    Nombre de fichiers : 4

    REPETOIRE APRES :
    Nom du fichier	 Taille		 Blocs de données
    fichier1	512 octets		6 8 9 11
    fichier2	380 octets		7 12 13
    fichier3	252 octets		14 15
    fichier6	378 octets		18 20 16

    ./test_mv_file fichier5 fichier7

    REPETOIRE AVANT:
    Nom du fichier	 Taille		 Blocs de données
    fichier1	512 octets		6 8 9 11
    fichier2	380 octets		7 12 13
    fichier3	252 octets		14 15
    fichier6	378 octets		18 20 16

    Nombre de fichiers : 4

    fichier5 n'existe pas


## 4. AFFICHAGE DU CONTENU D'UN FICHIER

    int cat_file (char* file) {
     /* A COMPLETER */
      int i, j = 0;
      struct ent_dir *pt = pt_DIR;
      short *fat_entry;
      char buffer[SIZE_SECTOR];

      // Cherche le fichier file dans le répertoire
      for (i = 0; i < NB_DIR; i++) {
          if ((pt->del_flag) && (!strcmp(pt->name, file))) {
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
              return 0;
          }
          pt++;
      }
      // Le fichier file n'a pas été trouvé
      return -1;
    }

Résultat:

    ./test_cat fichier2

    fichier2:
    222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222223333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

    ./test_cat fichier7

    fichier7:
    fichier7 n'existe pas
    Nom du fichier	 Taille		 Blocs de données
    fichier1	512 octets		6 8 9 11
    fichier2	380 octets		7 12 13
    fichier3	252 octets		14 15
    fichier6	378 octets		18 20 16

    Nombre de fichiers : 4

## 5. SUPPRESSION D'UN FICHIER

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

Résultat:

    ./test_del_file fichier3

    REPETOIRE AVANT :
    Nom du fichier	 Taille		 Blocs de données
    fichier1	512 octets		6 8 9 11
    fichier2	380 octets		7 12 13
    fichier3	252 octets		14 15
    fichier6	378 octets		18 20 16

    Nombre de fichiers : 4
    FAT AVANT : 0 1 2 3 6 7 8 9 11 12 13 14 15 16 18 20

    EFFACER fichier3

    REPETOIRE APRES :
    Nom du fichier	 Taille		 Blocs de données
    fichier1	512 octets		6 8 9 11
    fichier2	380 octets		7 12 13
    fichier6	378 octets		18 20 16

    Nombre de fichiers : 3
    FAT APRES : 0 1 2 3 6 7 8 9 11 12 13 15 16 18 20

    ./test_del_file fichier7

    REPETOIRE AVANT :
    Nom du fichier	 Taille		 Blocs de données
    fichier1	512 octets		6 8 9 11
    fichier2	380 octets		7 12 13
    fichier6	378 octets		18 20 16

    Nombre de fichiers : 3
    FAT AVANT : 0 1 2 3 6 7 8 9 11 12 13 15 16 18 20

    EFFACER fichier7
    fichier7 n'existe pas



## 6. CREATION D'UN FICHIER VIDE

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

Résultat:

    ./test_create_file fichier5

    REPETOIRE AVANT :
    Nom du fichier	 Taille		 Blocs de données
    fichier1	512 octets		6 8 9 11
    fichier2	380 octets		7 12 13
    fichier6	378 octets		18 20 16

    Nombre de fichiers : 3
    cree fichier fichier5

    REPETOIRE APRES :
    Nom du fichier	 Taille		 Blocs de données
    fichier1	512 octets		6 8 9 11
    fichier5	0 octets		
    fichier2	380 octets		7 12 13
    fichier6	378 octets		18 20 16

    Nombre de fichiers : 4

    ./test_create_file fichier2

    REPETOIRE AVANT :
    Nom du fichier	 Taille		 Blocs de données
    fichier1	512 octets		6 8 9 11
    fichier5	0 octets		
    fichier2	380 octets		7 12 13
    fichier6	378 octets		18 20 16

    Nombre de fichiers : 4
    cree fichier fichier2
    fichier2 existe deja


## 7. ALLOCATION D'UN BLOC DE DONNEES

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

Résultat:

    FAT avant :0 1 2 3 4 6 7 8 9 11 12 13 15 16 18 20
    bloc alloue : 5
    FAT apres :0 1 2 3 4 5 6 7 8 9 11 12 13 15 16 18 20
