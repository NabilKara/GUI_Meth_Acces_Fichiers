#ifndef INDEX_H_INCLUDED
#define INDEX_H_INCLUDED

#include<stdlib.h>
#include<TŌVC.h>
#include<TŌVC.c>

// declarations des constantes 
#define MAX_INDEX 1000// taille de la table d'index
// max index est a revoir plus tard , je sais pas encore comment recuperer le nombre de cle dans un fichier
#define MAX_INDEX_TOF 4 // taille du buffer du fichier d'index

// Numeros des entetes 

#define ENTETE_NUMERO_DERNIER_BLOC_TOF 1
#define ENTETE_NOMBRE_ENREGISTREMENTS_TOF 2

typedef struct 
{
    char cle[TAILLE_CLE];
    int numBloc;
    int posBloc;
}DataIndex;

typedef struct 
{
    DataIndex tab[MAX_INDEX_TOF];
    int nbIndex;  // nombre d'index inseres
}Bloc_TOF,Buffer_TOF;

typedef struct 
{
    DataIndex tab[MAX_INDEX];
    int taille;
}TableIndex;

typedef struct 
{
    int numDernierBloc;
    int nbEnreg; // nombre d'enregistrements inseres
}Entete_TOF;

typedef struct 
{
    Entete_TOF entete;
    FILE* fichier;
}TOF;
// variables globales 
TableIndex *tabIndex;  // table d'index generale
bool tabIndexExis; // verifier si la table d'index existe

/*------Declarations des fonctions-------------*/

/*------Fonction du modele abstrait-------------*/

bool ouvrir_TOF(TOF* f,char nom_f[],char mode);
bool fermer_TOF(TOF* f);
bool lireBloc_TOF(TOF* f,int i,Buffer *buf);
bool ecrireBloc_TOF(TOF* f,int i,Buffer* buf);
int entete_TOF(TOF* f,int i);
bool affecterEntete_TOF(TOF* f,int i,int val);
bool allouerBloc_TOF(TOF* f);

/*----------fonctions liee au traitement sur la table d'index-----------*/

TableIndex* alloc_TabIndex(); // allouer un espace en MC pour la table d'index
void liberer_TabIndex(TableIndex* t); // liberer l'espace occupe par la table d'index
bool charger_TabIndex(char nom_fich[],TableIndex* t); // charger une table d'index a partir du fichier
bool sauvegarder_TabIndex(char nom_fich[],TableIndex* t); // sauvegarder une table d'index dans un fichier

//*****fonctions necessaires aux traitements****

bool lire_chaine_TOF(TOF* f,Buffer* buf,int* i,int* j,int taille,char *ch[]); // lire une chaine de caracteres a partir du fichier
bool ecrire_chaine_TOF(TOF* f,Buffer* buf,int* i,int *j,int taille,char ch[]);// ecrire une chaine de caracteres au fichier

#endif INDEX_H_INCLUDED