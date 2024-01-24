#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include<stdbool.h>
#include "index.h"
#include "TnOVC.h"
#include "utils.h"
#include "utils.c"
#include "index.c"

/*---Operations fondamentales sur les fichiers---*/

bool rechercher(char nom_fichier[], char cle[20], int *i, int *j);
bool inserer(char e[], int taille, char nom_fichier[]);
bool suppression_logique(char cle[20], char nom_fichier[]);

int rechercherIndex(TableIndex* t, char cle[TAILLE_CLE],bool *trouv);
bool creerTableIndex(char nom_fich[]);

#endif //FUNCTIONS_H_INCLUDED 
