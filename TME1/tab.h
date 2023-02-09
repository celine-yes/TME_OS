#ifndef TAB_H
#define TAB_H

#define NMAX 10

void InitTab(int *tab, int size);
void PrintTab(int *tab, int size);
int SumTab(int *tab, int size);
int MinSumTab(int *min, int *tab, int size);

#endif