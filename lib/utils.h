#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include"TnOVC.h"
/*---fonctions necessaires au traitement*/

int strToInt(char ch[]);
void intToStr(int n, char chaine[], int taille) ;
void recupererStr(char source[] , int debut , char dest[]);
int nbPos(int n) ;
void clearString(char *str, size_t size);
#endif