# TME9 - REMPLACEMENT DE PAGES

## 1. ECRITURE D’UNE STRATEGIE DE REMPLACEMENT FIFO

##### Résultat

    Page 1 referenced
    frame_nb = 3
    res = 0
    /!\ PAGE FAULT !!! /!\
    Frame 0 has been choosen
    (frame 0: 1) (frame 1: _) (frame 2: _)

    Page 2 referenced
    frame_nb = 3
    res = 1
    /!\ PAGE FAULT !!! /!\
    Frame 1 has been choosen
    (frame 0: 1) (frame 1: 2) (frame 2: _)

    Page 3 referenced
    frame_nb = 3
    res = 2
    /!\ PAGE FAULT !!! /!\
    Frame 2 has been choosen
    (frame 0: 1) (frame 1: 2) (frame 2: 3)

    Page 1 referenced
    (frame 0: 1) (frame 1: 2) (frame 2: 3)

    Page 2 referenced
    (frame 0: 1) (frame 1: 2) (frame 2: 3)

    Page 4 referenced
    frame_nb = 3
    res = 0
    /!\ PAGE FAULT !!! /!\
    Frame 0 has been choosen
    (frame 0: 4) (frame 1: 2) (frame 2: 3)

    Page 1 referenced
    frame_nb = 3
    res = 1
    /!\ PAGE FAULT !!! /!\
    Frame 1 has been choosen
    (frame 0: 4) (frame 1: 1) (frame 2: 3)

    Page 2 referenced
    frame_nb = 3
    res = 2
    /!\ PAGE FAULT !!! /!\
    Frame 2 has been choosen
    (frame 0: 4) (frame 1: 1) (frame 2: 2)

    Page 4 referenced
    (frame 0: 4) (frame 1: 1) (frame 2: 2)

    Page 4 referenced
    (frame 0: 4) (frame 1: 1) (frame 2: 2)

    6/10 ~ 60.000000%

## 1. ECRITURE D’UNE STRATEGIE DE REMPLACEMENT LRU

##### Résultat

    Page 1 referenced
    /!\ PAGE FAULT !!! /!\
    Frame 0 has been choosen
    (frame 0: 1) (frame 1: _) (frame 2: _)

    Page 2 referenced
    /!\ PAGE FAULT !!! /!\
    Frame 1 has been choosen
    (frame 0: 1) (frame 1: 2) (frame 2: _)

    Page 3 referenced
    /!\ PAGE FAULT !!! /!\
    Frame 2 has been choosen
    (frame 0: 1) (frame 1: 2) (frame 2: 3)

    Page 1 referenced
    (frame 0: 1) (frame 1: 2) (frame 2: 3)

    Page 2 referenced
    (frame 0: 1) (frame 1: 2) (frame 2: 3)

    Page 4 referenced
    /!\ PAGE FAULT !!! /!\
    Frame 2 has been choosen
    (frame 0: 1) (frame 1: 2) (frame 2: 4)

    Page 1 referenced
    (frame 0: 1) (frame 1: 2) (frame 2: 4)

    Page 2 referenced
    (frame 0: 1) (frame 1: 2) (frame 2: 4)

    Page 4 referenced
    (frame 0: 1) (frame 1: 2) (frame 2: 4)

    Page 4 referenced
    (frame 0: 1) (frame 1: 2) (frame 2: 4)

    4/10 ~ 40.000000%

## 3. COMPARAISON LRU /FIFO

D'après les résultats obtenues, on peut observer que FIFO performe mieux que LFU.
En effet, le taux de réussite de FIFO est plus élevé que celle de LRU (60% > 40%).

***Analyse résultat obtenues par FIFO:***
- Les trois premières pages entrainent un Page Fault, ce qui est normal car aucune frame n'est encore allouée. Les pages 1, 2 et 3 sont attribués par les frames 0, 1 et 2 respectivement.
- Les pages 1 et 2 sont déjà en mémoire, il n'y donc pas de Page Fault.
- La page 4 n'est pas en mémoire, donc Page Fault, et la stratégie FIFO lui attribue la frame la plus ancienne en mémoire, qui est la frame 0 qui était attribué à la page 1.
- la page 1 n'est plus en mémoire, il y a donc un Page Fault. La stratégie FIFO lui attribue la frame la plus ancienne en mémoire, qui est la frame 1 qui était attribué à la page 2.
- La page 2 n'est plus en mémoire -> Page Fault. Stratégie FIFO lui attribue la frame la plus ancienne en mémoire, qui est la frame 1 qui était attribué à la page 3
- page 4 en mémoire donc pas de Page Fault.  
=> implémentation correcte de FIFO



***Analyse résultat obtenues par LRU:***
- Les trois premières pages entrainent un Page Fault, ce qui est normal car aucune frame n'est encore allouée. Les pages 1, 2 et 3 sont attribués par les frames 0, 1 et 2 respectivment.
- Les pages 1 et 2 sont déjà en mémoire, il n'y donc pas de Page Fault.
L'âge des frames (0 et 1) attribués aux pages 1 et 2 est donc actualisé.
- La page 4 n'est pas en mémoire, donc Page Fault, et la stratégie LRU lui attribue la frame ayant l'âge le plus bas qui est donc la frame 2.
- Les pages 1, 2 et 4 sont en mémoire donc pas de Page Fault, l'âge de leur frame est actualisé.
=> implémentation correcte de LRU
