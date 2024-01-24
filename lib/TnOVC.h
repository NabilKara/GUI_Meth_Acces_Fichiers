#ifndef TnOVC_H_INCLUDED
#define TnOVC_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>
/*---Constantes---*/
#define MAX_NO_CHARS 99 //le nombre maximal des caracteres dans un bloc

// Numero des entetes
#define ENTETE_NUMERO_DERNIER_BLOC 1
#define ENTETE_POSLIBRE_DERNIER_BLOC 2
#define ENTETE_NOMBRE_CHAR_SUP 3
#define ENTETE_NOMBRE_ENREGISTREMENTS 4

// representation d'un enregistrement
#define TAILLE_EFFECTIVE_ENREG 3 // la taille effective de la donne dans l'enregistrement est sur 3 octects
#define TAILLE_CHAR_EFFACEMENT_LOGIQUE 1 // "E": pour efface , "N" pour: non efface
#define TAILLE_CLE 20
// n le reste

/*--Structures--*/

typedef struct
{
    char tab[MAX_NO_CHARS + 1];
    int nb;
}Bloc, Buffer;

typedef struct
{
    int numeroDernierBloc;
    int positionLibreDernierBloc;
    int nbCharSupp;
    int nbEnreg;
}Entete;
typedef struct
{
    Entete entete;
    FILE* fichier;
}TnOVC;


/*---Fonctions du modele abstrait---*/
//*****fonctions propres au modele****
bool ouvrir(TnOVC* f,char nom_f[],char mode);
bool fermer(TnOVC* f);
bool lireBloc(char nom_fichier[],int i,Buffer *buf);
bool ecrireBloc(char nom_f[],int i, Buffer* buf);
int entete(TnOVC* f,int i);
bool affecterEntete(TnOVC* f,int i,int val);
int allouerBloc(TnOVC* f);
int entete2(int INDEX,char nom_fichier[]);
//==========new
Entete readHeader();
void setHeader(int INDEX, int val);
int getHeader(int INDEX);
//*****fonctions necessaires aux traitements****
bool lire_chaine(char nom_fichier[],Buffer* buf,int* i,int* j,int taille,char ch[]); // lire une chaine de caracteres a partir du fichier
bool ecrire_chaine(char nom_fichier[],Buffer* buf,int* i,int *j,int taille,char *ch);// ecrire une chaine de caracteres au fichier
#endif // TnOVC_H_INCLUDED