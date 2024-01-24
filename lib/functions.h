#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include<stdbool.h>
#include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/index.h"
#include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/TnOVC.h"
#include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/utils.h"
#include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/utils.c"
#include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/index.c"

/*---Operations fondamentales sur les fichiers---*/

bool rechercher(char nom_fichier[], char cle[20], int *i, int *j);
bool inserer(char e[], int taille, char nom_fichier[]);
bool suppression_logique(char cle[20], char nom_fichier[]);

int rechercherIndex(TableIndex* t, char cle[TAILLE_CLE],bool *trouv);
bool creerTableIndex(char nom_fich[]);

#endif //FUNCTIONS_H_INCLUDED 
