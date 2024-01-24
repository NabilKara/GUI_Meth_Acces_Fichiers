#include<stdio.h>
#include<stdlib.h>
#include <string.h>

#include "utils.h"

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
 * @brief retourner le nombre de position d'un entier
 * 
 * @param n l'entier a calculer le nbPos
 * @return int
 */
int nbPos(int n)
{
    int pos = 1;

    while (n / 10 != 0)
    {
        pos++;
        n /= 10;
    }

    return pos;
}


/**
 * @brief convertir un entier a une chaine de caractere
 * 
 * @param n l'entier a convertir
 * @param ch resultat de la conversion
 * @param taille TAILLE_EFFECTIVE_ENREG
 */
void intToStr(int n, char chaine[], int taille)
{
    const int ZEROS = taille - nbPos(n);
    if (ZEROS < 0)
    {
        printf("[ERROR] la taille de l'entier a convertir vers une chaine de caractere est plus grande que la taille de la chaine !!\n");
        strcpy(chaine, "");
        return;
    }

    char *temp_str = calloc(taille + 1, sizeof(char));
    sprintf(temp_str, "%d", n);

    for (int i = 0; i < ZEROS; i++)
        chaine[i] = '0';
    chaine[ZEROS] = '\0';

    strcat(chaine, temp_str);
    free(temp_str);
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

void clearString(char *str, size_t size) {
    if (str == NULL || size == 0) {
        // Handle invalid input
        return;
    }

    // Fill the string with null characters
    memset(str, '\0', size);
}