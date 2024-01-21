#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "TnOVC.h"
int main()
{
    TnOVC* f = malloc(sizeof(TnOVC));

    if(!ouvrir(f,"testFiles/test.txt",'n')) printf("Error opening the file");
    Buffer *buf = malloc(sizeof(Buffer));

    char ch[] = "premier TEST" ;

    int i = allouerBloc(f), j = 0;

    if (i == -1)
        printf("i = -1");

    if (!ecrire_chaine(f, buf, &i, &j, strlen(ch), ch))
    {
        printf("Erreur d'ecriture de chaine dans le fichier !!!!!\n");
        exit(1);
    }

    if (!ecrireBloc(f, i, buf))
    {
        printf("Erreur d'ecrire bloc de fichier !!!!!\n");
    }
    // Close the file after writing
    fermer(f);
    printf("\n le programme marche tres bien \n");

    return 0;
}
