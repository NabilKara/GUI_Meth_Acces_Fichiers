#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#endif

#include<stdbool.h>

/*---Operations fondamentales sur les fichiers---*/

bool rechercher(char nom_fichier[],char cle[20],int *i,int *j);
bool inserer(char e[],int taille,char nom_fichier[]);
bool suppression_logique(char cle[20], char nom_fichier[]);

