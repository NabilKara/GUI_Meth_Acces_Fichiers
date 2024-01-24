#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>


#include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/functions.h"
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
    
    TnOVC fichier;
    Buffer buf;

    if (!ouvrir(&fichier,nom_fichier,'a'))
    {
        return false ;
    }

    int N = entete(&fichier, ENTETE_NUMERO_DERNIER_BLOC);
    int M = entete(&fichier,ENTETE_POSLIBRE_DERNIER_BLOC);

    if ( N <= 0) //le cas où le fichier est vide
    {
        fermer(&fichier);
        return false;
    }

    *i = 1;
    *j = 0;
    
    lireBloc(&fichier,*i,&buf);

    while (*i < N ||  *j != M) 
    {   
        char *chLong[TAILLE_EFFECTIVE_ENREG];
        char *chEff[TAILLE_CHAR_EFFACEMENT_LOGIQUE];
        char *chCle[TAILLE_CLE];

        lire_chaine(&fichier,&buf,i,j,TAILLE_EFFECTIVE_ENREG,chLong);
        lire_chaine(&fichier,&buf,i,j,TAILLE_CHAR_EFFACEMENT_LOGIQUE,chEff);
        lire_chaine(&fichier,&buf,i,j,TAILLE_CLE,chCle);
                
        if((strcmp(chCle[0],cle) == 0) && (strcmp(chEff[0],"N") == 0)){
            return true;
        }else{
            *j = *j +  strToInt(*chLong) - TAILLE_CLE;
            if(*j > MAX_NO_CHARS){
                // chevauchement
                *j -= MAX_NO_CHARS;
                (*i)++;
                lireBloc(&fichier,*i,&buf);
            }
        }
    }
    fermer(&fichier);
    
    return false;
}

/**
 * @brief inserer un enregistrement dans un fichier TnOVC
 * 
 * @param e la donnee a inserer  a inserer
 * @param taille taille effective de la donnee
 * @param nom_fichier nom du fichier
 * @return true 
 * @return false 
 */
bool inserer(char e[], int taille, char nom_fichier[]){
    Buffer buf;
    TnOVC f;
    int i,j;
    if(!ouvrir(&f,nom_fichier,'a')) return false;
    i = entete(&f,ENTETE_NUMERO_DERNIER_BLOC); // numero du dernier bloc
    
    if(i == 0){
        return false;
    }

    j = entete(&f,ENTETE_POSLIBRE_DERNIER_BLOC); // position libre dernier bloc
    
    char ch[3];
    intToStr(taille,ch,3);
    
    ecrire_chaine(&f,&buf,&i,&j,TAILLE_EFFECTIVE_ENREG,ch);
    ecrire_chaine(&f,&buf,&i,&j,1,"N");
    ecrire_chaine(&f,&buf,&i,&j,taille+TAILLE_CLE,e);

    ecrireBloc(&f,i,&buf);

    if(i != entete(&f,1)) affecterEntete(&f,ENTETE_NUMERO_DERNIER_BLOC,i);
    affecterEntete(&f,ENTETE_POSLIBRE_DERNIER_BLOC,j);
    fermer(&f);
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
    TnOVC fichier;
    Buffer buf;
    char *ch[TAILLE_EFFECTIVE_ENREG];
    int i,j;
    if(rechercher(nom_fichier,cle,&i,&j)){
        if (!ouvrir(&fichier,nom_fichier,'a'))
        {
            return false ;
        }
        lireBloc(&fichier,i,&buf);
        j -= TAILLE_EFFECTIVE_ENREG +TAILLE_CLE + 1;
        lire_chaine(&fichier,&buf,&i,&j,TAILLE_EFFECTIVE_ENREG,ch);
        if(j <= MAX_NO_CHARS){
             buf.tab[j] = 'E';  // positioner le caractere d'effacement logique
        }else{
            //chevauchement
            i++;
            lireBloc(&fichier,i,&buf);
            buf.tab[1] = 'E';
        }
    ecrireBloc(&fichier,i,&buf);
    // mettre a jour le caractere indiquant le nombre de char logiquements supprime
    affecterEntete(&fichier,ENTETE_NOMBRE_CHAR_SUP,entete(&fichier,ENTETE_NOMBRE_CHAR_SUP)+ strToInt(*ch)+ TAILLE_EFFECTIVE_ENREG
                                                                            + TAILLE_CHAR_EFFACEMENT_LOGIQUE);
    fermer(&fichier);
    }
}


/**
 * @brief rechercher un enregistrement dans la table d'index a base de son cle
 * 
 * @param t la table d'index
 * @param cle identifiant de l'enregistrement
 * @return int 
 */
int rechercherIndex(TableIndex* t,char cle[TAILLE_CLE]){
    if(t == NULL){
        return -1;
    }else{
        int bi = 0, bs = t->taille;
        bool trouv = false;
        int mid ;
        while(bi <= bs && !trouv){
            mid = (bi+bs)/2;
            int cmp = strcmp(t->tab[mid].cle,cle);
            if(!cmp) {
                trouv = true;
            }else if(cmp < 0){ // cle > cle de la table d'index
                bi = mid+1;
            }else{
                bs = mid-1;
            }
        }
        return mid;
    }
}


/**
 * @brief creer la table d'index
 *  
 * @param nom_fich nom du fichier
 * @return true 
 * @return false 
 */
bool creerTableIndex(char nom_fich[])
{
    tabIndexExis = false ;
    TnOVC *f;
    if (!ouvrir(f,nom_fich,'A')) // ouvrir le fichier au mode ancien
    {
        printf("Erreur de l'ouverture de fichier");
        return false;
    }

    // fichier TnOVC vide
    if (entete(f, ENTETE_NUMERO_DERNIER_BLOC) <= 0)
    {
        fermer(f);
        return false;
    }

    tabIndex = alloc_TabIndex();
    tabIndex->taille = 0 ;
    Buffer *buf ;
    DataIndex data;
    char position[TAILLE_EFFECTIVE_ENREG];
    int i = 1 ;
    int j = 0 ;
    while (i <= f->entete.numeroDernierBloc)
    {
        if(!lireBloc(f,i,buf))
        {
            return false;
        }
        strncpy(data.cle,buf->tab,TAILLE_CLE);
        data.numBloc = i ;
        recupererStr(buf->tab,TAILLE_CLE+1,position) ;
        data.posBloc = TAILLE_CLE + 1 + TAILLE_EFFECTIVE_ENREG + strToInt(position) ;
        if(!updateTableIndex(data,tabIndex,'A'))
        {
            return false;
        }
        i++ ;
    }

    tabIndexExis = true ;
    return tabIndexExis ;
}
