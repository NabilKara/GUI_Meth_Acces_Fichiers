#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "TŌVC"
#include "index.h"
#include "functions.h"
#include "utils.h"

int main()
{
    TŌVC *f = malloc(sizeof(TŌVC)) ;
    Buffer *buf = malloc(sizeof(Buffer));

    char ch[] = "premier TEST" ;

    if (!ouvrir(f, "test.bin", 'N'))
    {
        printf("Erreur d'ouverture de fichier !!!!!\n");
        exit(1);
    }

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
