#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<utils.h>
#include"/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/TnOVC.h"

/**
 * @brief Convertir une chaine de caractere a un entier
 * 
 * @param ch la chaine a convertir
 * @return int 
 */
int strToInt(char ch[])
{
    return atoi(ch);
}

/**
 * @brief convertir un entier a une chaine de caractere
 * 
 * @param n l'entir a convertir
 * @param ch resultat de la conversion
 */
void intToStr(int n,char ch[])
{ 
    if(n < 0 || n > MAX_NO_CHARS)
    {
        printf("ERREUR !!!!");
        exit(1) ;
    }
    else
    {
        sprintf(ch , "%d" , n) ;
    }
}
/**
 * @brief copier une chaine de caractere
 * 
 * @param source chaine source
 * @param debut indice de debut
 * @param dest  chaine destination
 */
void recupererStr(char source[MAX_NO_CHARS] , int debut , char dest[TAILLE_EFFECTIVE_ENREG])
{
    int j = 0 ;
    for (int i = debut; i < debut + TAILLE_EFFECTIVE_ENREG ; i++)
    {
        dest[j] = source[i] ;
        j++ ;
    }
    
}