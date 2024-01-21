#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
// #include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/includes.h"
#include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/TnOVC.h"
#include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/TnOVC.c"
// #include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/index.h"
// #include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/index.c"
#include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/functions.h"
#include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/functions.c"
int main()
{
    TnOVC* f = malloc(sizeof(TnOVC));
    Buffer* buff;
    
    if(!ouvrir(f,"testFiles/test.bin",'n')) printf("Error opening the file");
    Buffer *buf = malloc(sizeof(Buffer));
    printf("%d", f->entete.numeroDernierBloc);
    printf("%d", f->entete.positionLibreDernierBloc);
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
    printf("%d", f->entete.numeroDernierBloc);
    printf("%d", f->entete.positionLibreDernierBloc);

    char e[] = "@.,s,dfsslaskdro9012NABILKARA";
    if(!inserer(e,9,"test.bin")) printf("Erreur lors de l'insertion dans le fichier\n");
    // Close the file after writing
    fermer(f);
    printf("\n le programme marche tres bien \n");
    return 0;
}
