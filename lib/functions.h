#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#endif

#include<stdbool.h>
#include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/index.h"

/*---Operations fondamentales sur les fichiers---*/

bool rechercher(char nom_fichier[],char cle[20],int *i,int *j);
bool inserer(char e[],int taille,char nom_fichier[]);
bool suppression_logique(char cle[20], char nom_fichier[]);

int rechercherIndex(TableIndex* t,char cle[TAILLE_CLE]);
bool creerTableIndex(char nom_fich[]);
