#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
// #include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/includes.h"
#include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/TnOVC.h"
#include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/TnOVC.c"
#include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/functions.h"
#include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/functions.c"
int main()
{
    // TnOVC* f = malloc(sizeof(TnOVC));
    // char nom_fichier[] = "testFiles/testing.bin";
    // if(!ouvrir(f,nom_fichier,'n')) printf("Error opening the file\n");
    // Buffer *buf = malloc(sizeof(Buffer));
    // char ch[] = "premier TEST" ;

    // int i = allouerBloc(f), j = 0;
    // if (i == -1)
    //     printf("i = -1");

    // if (!ecrire_chaine(f, buf, &i, &j, strlen(ch), ch))
    // {
    //     printf("Erreur d'ecriture de chaine dans le fichier !!!!!\n");
    //     exit(1);
    // }

    // if (!ecrireBloc(f, i, buf))
    // {
    //     printf("Erreur d'ecrire bloc de fichier !!!!!\n");
    // }
    // fermer(f);
    
    // char e1[] = "CleNum1DuEnregOuiCcaNABILKARA";
    // char e2[] = "askfjasfsalnfksanfasSIFITAREK";
    // if(!inserer(e1,9,nom_fichier)) printf("Erreur lors de l'insertion de e1 dans le fichier\n");
    // if(!inserer(e2,9,nom_fichier)) printf("Erreur lors de l'insertion de e1 dans le fichier\n");
    // char cle1[21] = "CleNum1DuEnregOuiCca";
    
    // if(!suppression_logique(cle1,nom_fichier)) printf("Erreur suppression\n");

     
    // printf("\n le programme marche tres bien \n");
    char nomfichier[] = "testFiles/fichierTOF.bin";
    TOF *file;
    file = malloc(sizeof(TOF));
    ouvrir_TOF(file,nomfichier,'N');
    Buffer_TOF *buf_TOF;
    buf_TOF = malloc(sizeof(Buffer_TOF));
    int k = allouerBloc_TOF(file);
    if(k == -1){
        printf("k = -1\n");
    }
    char cle[] = "147321538";
    buf_TOF->nbIndex = 1;
    int numBloc = 1;
    int posBloc = 1;
    DataIndex dataIdx;
    dataIdx.numBloc = numBloc;
    dataIdx.posBloc = posBloc;
    strcpy(dataIdx.cle,cle);
    buf_TOF->tab[0] = dataIdx;
    if(!ecrireBloc_TOF(file,k,buf_TOF)) printf("Erreur lors de l'ecriture dans le fichier TOF\n");

    // creer une table d'index
    TableIndex* tableIdx = alloc_TabIndex();
    tableIdx->tab[0] = dataIdx;
    tableIdx->taille++;
    
    // sauvegarder la table d'index dans un fichier
    if(!sauvegarder_TabIndex("testFiles/fichierTOF.bin",tableIdx)) {
        printf("Erreur lors du sauvegarde de la table d'index\n");
        exit(1);
    }
    liberer_TabIndex(&tableIdx);
    tableIdx = alloc_TabIndex();
    

    if(!charger_TabIndex(nomfichier,tableIdx)) printf("Erreur de chargement de la table d'index\n");
    
    printf("cle : %s\n",tableIdx->tab[0].cle);
    printf("numBloc: %d\n",tableIdx->tab[0].numBloc);
    printf("posBlocL %d\n",tableIdx->tab[0].posBloc);

    printf("Test fini avec succes\n");
    return 0;
}
