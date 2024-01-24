#include "GUI.h"
bool recupererEnreg(char nom_fichier[],char ch[],int *numBlc,int *depl){
    TnOVC f;
    Buffer buf;
    char cle[TAILLE_CLE];
    char length[TAILLE_EFFECTIVE_ENREG];
    
    int taille;
    
    if(!ouvrir(&f,nom_fichier,'a')) return false; 
    
    lireBloc(&f,*numBlc,&buf);

    
    if(*numBlc > entete(&f,ENTETE_NUMERO_DERNIER_BLOC)) return false;
    
    lire_chaine(&f,&buf,numBlc,depl,TAILLE_EFFECTIVE_ENREG,length);

    taille = strToInt(ch);

    // si on veut recuperer aussi les caracteres de controle
    *depl -= TAILLE_EFFECTIVE_ENREG;
    lire_chaine(&f,&buf,numBlc,depl,taille+TAILLE_CHAR_EFFACEMENT_LOGIQUE+TAILLE_CLE+TAILLE_EFFECTIVE_ENREG,ch);
    
    // si on veut juste recuperer la cle et la donne
    // *depl += TAILLE_CHAR_EFFACEMENT_LOGIQUE;
    // lire_chaine(&f,&buf,numBlc,depl,taille+TAILLE_CLE,ch);
    
    // si on veut recuperer la cle et la donnee separement donc ce cas ajouter une variable char cle[] au prototype de la fonction
    // *depl += TAILLE_CHAR_EFFACEMENT_LOGIQUE;
    // lire_chaine(&f,&buf,numBlc,depl,TAILLE_CLE,cle);
    // lire_chaine(&f,&buf,numBlc,depl,taille,ch);
    return true;
}