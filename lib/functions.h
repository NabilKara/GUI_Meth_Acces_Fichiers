#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include<stdbool.h>
#include "index.h"

/*---Operations fondamentales sur les fichiers---*/

bool rechercher(char nom_fichier[],char cle[TAILLE_CLE],int *i,int *j);
bool inserer(char e[],int taille,char nom_fichier[],int cle);
bool suppression_logique(char cle[TAILLE_CLE], char nom_fichier[]);

int rechercherIndex(TableIndex* t, char cle[20]);
bool creerTableIndex(char nom_fich[]);

#endif //FUNCTIONS_H_INCLUDED 
