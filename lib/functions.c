#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include "functions.h"
//#include "index.h"
#include "utils.h"
#include "TnOVC.h"

/**
 * @brief rechercher un enregistrement a base d'une cle dans un fichier TnOVC
 * 
 * @param nom_fichier nom du fichier 
 * @param cle identifiant de l'enregistrement
 * @param i numero du bloc ou devrait se trouver l'enregistrement
 * @param j position dans le bloc ou devrait se trouver l'enregistrement
 * @return true 
 * @return false 
 */
bool rechercher(char nom_fichier[],char cle[TAILLE_CLE],int *i,int *j){
    TnOVC* fichier;
    Buffer* buf;
    if (!ouvrir(fichier,nom_fichier,'a'))
    {
        return false ;
    }
    
    *i = 0;
    *j = 0;
    while (*i < entete(ENTETE_NUMERO_DERNIER_BLOC,nom_fichier) &&  *j != entete(ENTETE_POSLIBRE_DERNIER_BLOC,nom_fichier)) 
    {   
        char *chLong;
        char *chEff;
        char *chCle;

        lire_chaine(nom_fichier,buf,i,j,TAILLE_EFFECTIVE_ENREG,chLong);
        lire_chaine(nom_fichier,buf,i,j,TAILLE_CHAR_EFFACEMENT_LOGIQUE,chEff);
        lire_chaine(nom_fichier,buf,i,j,TAILLE_CLE,chCle);
        
        if(((memcmp(chCle,cle,TAILLE_CLE)) == 0) && (strcmp(chEff,"N"))){
            return true;
        }else{
            *j = *j +  strToInt(chLong) - TAILLE_CLE;
            if(*j > MAX_NO_CHARS){
                // chevauchement
                *j -= MAX_NO_CHARS;
                (*i)++;
                lireBloc(nom_fichier,*i,buf);
            }
        }
    }
    fermer(fichier);
    
    return false;
}

/**
 * @brief inserer un enregistrement dans un fichier TnOVC
 * 
 * @param e la donnee a inserer  a inserer
 * @param taille taille effective de la donnee
 * @param nom_fichier nom du fichier
 * @param cle la cle 
 * @return true 
 * @return false 
 */
bool inserer(char e[],int taille,char nom_fichier[],int cle){
    TnOVC* fichier=malloc(sizeof(TnOVC));
    Buffer *buf = malloc(sizeof(Buffer));
    
    lireBloc(nom_fichier,entete(ENTETE_NUMERO_DERNIER_BLOC,nom_fichier),buf);

    
    if (!ouvrir(fichier,nom_fichier,'A'))
    {
        return false ;
    }
    
    int i = entete(ENTETE_NUMERO_DERNIER_BLOC,nom_fichier);
    int j = entete(ENTETE_POSLIBRE_DERNIER_BLOC,nom_fichier);

    char c[TAILLE_EFFECTIVE_ENREG];
    intToStr(taille,c,TAILLE_EFFECTIVE_ENREG);//c="taille"
    char id[TAILLE_CLE];
    intToStr(cle,id,TAILLE_CLE);


    ecrire_chaine(nom_fichier,buf,&i,&j,TAILLE_CLE,id);
    ecrire_chaine(nom_fichier,buf,&i,&j,TAILLE_CHAR_EFFACEMENT_LOGIQUE,"N");
    ecrire_chaine(nom_fichier,buf,&i,&j,TAILLE_EFFECTIVE_ENREG,c);
    ecrire_chaine(nom_fichier,buf,&i,&j,taille,e);
    
    ecrireBloc(nom_fichier,i,buf);
    if(i != entete(ENTETE_NUMERO_DERNIER_BLOC,nom_fichier)){
        affecterEntete(ENTETE_NUMERO_DERNIER_BLOC,i,nom_fichier);
        // mettre a jour le numero du dernier bloc
    }

    affecterEntete(ENTETE_POSLIBRE_DERNIER_BLOC,j,nom_fichier);
    // mettre a jour la premiere position libre dans le dernier bloc

    fclose(fichier->fichier);
    return true;
}


/**
 * @brief supprimer un enregistrement d'un fichier TnOVC a base de son cle
 * 
 * @param cle identifiant de l'enregistrement
 * @param nom_fichier nom du fichier
 * @return true 
 * @return false 
 */
bool suppression_logique(char cle[TAILLE_CLE], char nom_fichier[]){
    TnOVC* fichier;
    Buffer* buf;
    char *ch;
    int i,j;
    if(rechercher(nom_fichier,cle,&i,&j)){
        if (!ouvrir(fichier,nom_fichier,'a'))
        {
            return false ;
        }
        lireBloc(nom_fichier,i,buf);
        lire_chaine(nom_fichier,buf,&i,&j,TAILLE_EFFECTIVE_ENREG,ch);
        if(j <= MAX_NO_CHARS){
             buf->tab[j] = 'E';  // positioner le caractere d'effacement logique
        }else{
            //chevauchement
            i++;
            lireBloc(nom_fichier,i,buf);
            buf->tab[0] = 'E';
        }
    ecrireBloc(nom_fichier,i,buf);
    // mettre a jour le caractere indiquant le nombre de char logiquements supprime
    affecterEntete(entete(ENTETE_NOMBRE_CHAR_SUP,nom_fichier),entete(ENTETE_NOMBRE_CHAR_SUP,nom_fichier)+ strToInt(ch)+ TAILLE_EFFECTIVE_ENREG
                                                                            + TAILLE_CHAR_EFFACEMENT_LOGIQUE,nom_fichier);
    fermer(fichier);
    }
}


// /**
//  * @brief rechercher un enregistrement dans la table d'index a base de son cle
//  * 
//  * @param t la table d'index
//  * @param cle identifiant de l'enregistrement
//  * @return int 
//  */
// int rechercherIndex(char cle[TAILLE_CLE]){
    
//     if(tabIndex == NULL){
//         return -1;
//     }else{
//         int bi = 0, bs = tabIndex->taille;
//         bool trouv = false;
//         int mid ;
//         while(bi <= bs && !trouv){
//             mid = (bi+bs)/2;
//             int cmp = strcmp(tabIndex->tab[mid].cle,cle);
//             if(!cmp) {
//                 trouv = true;
//             }else if(cmp < 0){ // cle > cle de la table d'index
//                 bi = mid+1;
//             }else{
//                 bs = mid-1;
//             }
//         }
//         return mid;
//     }
// }


// /**
//  * @brief creer la table d'index
//  *  
//  * @param nom_fich nom du fichier
//  * @return true 
//  * @return false 
//  */
// bool creerTableIndex(char nom_fich[])
// {
//     tabIndexExis = false ;
//     TnOVC *f;
//     if (!ouvrir(f,nom_fich,'A')) // ouvrir le fichier au mode ancien
//     {
//         printf("Erreur de l'ouverture de fichier");
//         return false;
//     }

//     // fichier TnOVC vide
//     if (entete(ENTETE_NUMERO_DERNIER_BLOC,nom_fich) <= 0)
//     {
//         fermer(f);
//         return false;
//     }

//     tabIndex = alloc_TabIndex();
//     tabIndex->taille = 0 ;
//     Buffer *buf ;
//     DataIndex data;
//     char position[TAILLE_EFFECTIVE_ENREG];
//     int i = 1 ;
//     int j = 0 ;
//     while (i <= entete(ENTETE_NUMERO_DERNIER_BLOC,nom_fich))
//     {
//         if(!lireBloc(nom_fich,i,buf))
//         {
//             return false;
//         }
//         strncpy(data.cle,buf->tab,TAILLE_CLE);
//         data.numBloc = i ;
//         recupererStr(buf->tab,TAILLE_CLE+1,position) ;
//         data.posBloc = TAILLE_CLE + 1 + TAILLE_EFFECTIVE_ENREG + strToInt(position) ;
//         if(!updateTableIndex(data,tabIndex,'A'))
//         {
//             return false;
//         }
//         i++ ;
//     }

//     tabIndexExis = true ;
//     return tabIndexExis ;
// }
