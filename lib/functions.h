#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include<stdbool.h>
#include "functions.h"


/*---Operations fondamentales sur les fichiers---*/

bool rechercher(char nom_fichier[], char cle[20], int *i, int *j);
bool inserer(char e[],int taille,char nom_fichier[],int cle);
bool suppression_logique(char cle[20], char nom_fichier[]);

//int rechercherIndex(char cle[20]);
//bool creerTableIndex(char nom_fich[]);

#endif //FUNCTIONS_H_INCLUDED 
