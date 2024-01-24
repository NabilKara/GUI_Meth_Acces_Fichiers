#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>


#include "functions.h"
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
    TnOVC* fichier = malloc(sizeof(TnOVC));
    Buffer* buf = malloc(sizeof(Buffer));
    
    if (!ouvrir(fichier,nom_fichier,'a'))
    {
        return false ;
    }

    *i = 1;
    *j = 0;
    while (*i <= getHeader(1)/*entete(fichier,ENTETE_NUMERO_DERNIER_BLOC)*/ &&  *j != getHeader(2)/*entete(fichier,ENTETE_POSLIBRE_DERNIER_BLOC)*/)
    {
        char chLong[TAILLE_EFFECTIVE_ENREG];
        char chEff[TAILLE_CHAR_EFFACEMENT_LOGIQUE];
        char chCle[TAILLE_CLE];
    
        lireBloc(nom_fichier,*i,buf);
        
        lire_chaine(nom_fichier,buf,&(*i),&(*j),TAILLE_CLE,chCle);
        int x = strToInt(chCle) ;
        
        lire_chaine(nom_fichier,buf,&(*i),&(*j),TAILLE_CHAR_EFFACEMENT_LOGIQUE,chEff);
        char y = chEff[0] ;
        char z = 'N';
        
        lire_chaine(nom_fichier,buf,&(*i),&(*j),TAILLE_EFFECTIVE_ENREG,chLong);
        
        char data[strToInt(chLong)] ;
        lire_chaine(nom_fichier,buf,&(*i),&(*j),strToInt(chLong),data);
        

        
        if(x == strToInt(cle) && y == z)
        {
            fclose(fichier->fichier);
            free(fichier);
            free(buf);
            return true ;
        }
            
        
    }
    fclose(fichier->fichier);
    free(fichier);
    free(buf);
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
bool inserer(char e[],int taille,char nom_fichier[],int cle){
    TnOVC* fichier=malloc(sizeof(TnOVC));
    Buffer *buf = malloc(sizeof(Buffer));
    
    lireBloc(nom_fichier,getHeader(1),buf);

    
    if (!ouvrir(fichier,nom_fichier,'A'))
    {
        return false ;
    }
    
    int i = getHeader(1);
    int j = getHeader(2);

    char c[TAILLE_EFFECTIVE_ENREG];
    intToStr(taille,c,TAILLE_EFFECTIVE_ENREG);//c="taille"
    char id[TAILLE_CLE];
    intToStr(cle,id,TAILLE_CLE);


    ecrire_chaine(nom_fichier,buf,&i,&j,TAILLE_CLE,id);
    ecrire_chaine(nom_fichier,buf,&i,&j,TAILLE_CHAR_EFFACEMENT_LOGIQUE,"N");
    ecrire_chaine(nom_fichier,buf,&i,&j,TAILLE_EFFECTIVE_ENREG,c);
    ecrire_chaine(nom_fichier,buf,&i,&j,taille,e);
    
    ecrireBloc(nom_fichier,i,buf);
    if(i != entete(fichier,ENTETE_NUMERO_DERNIER_BLOC)){
        setHeader(1,i);
        // mettre a jour le numero du dernier bloc
    }

    setHeader(2,j);
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
    TnOVC* fichier = malloc(sizeof(TnOVC));
    Buffer* buf = malloc(sizeof(Buffer));
    char copieCle[TAILLE_CLE] ;
    char ch[TAILLE_EFFECTIVE_ENREG];
    int i,j;
    if(rechercher(nom_fichier,cle,&i,&j)){
        if (!ouvrir(fichier,nom_fichier,'a'))
        {
            return false ;
        }
        if(i == 1)
        {
            lireBloc(nom_fichier,i,buf);
            j = 0 ;
            while(j <= MAX_NO_CHARS)
            {
                lire_chaine(nom_fichier,buf,&i,&j,TAILLE_CLE,copieCle);
                int x = strToInt(copieCle);
                lire_chaine(nom_fichier,buf,&i,&j,TAILLE_CHAR_EFFACEMENT_LOGIQUE,ch);
                char y = ch[0] ;
                char z = 'N' ;
                if(x == strToInt(cle) && y == z)
                {
                    buf->tab[j-1] = 'E' ;
                    ecrireBloc(nom_fichier,i,buf) ;
                    break;
                }
                char sh[MAX_NO_CHARS] ;
                lire_chaine(nom_fichier,buf,&i,&j,TAILLE_EFFECTIVE_ENREG,sh) ;
                lire_chaine(nom_fichier,buf,&i,&j,strToInt(sh),sh);
            }
        }
        else
        {
            //chevauchement
            int k = i ;
            i = i - 1 ;
            j = 0 ;
            while(i <= k)
            {
                lireBloc(nom_fichier,i,buf);
                lire_chaine(nom_fichier,buf,&i,&j,TAILLE_CLE,copieCle);
                int x = strToInt(copieCle);
                lire_chaine(nom_fichier,buf,&i,&j,TAILLE_CHAR_EFFACEMENT_LOGIQUE,ch);
                char y = ch[0] ;
                char z = 'N' ;
                if(x == strToInt(cle) && y == z)
                {
                    buf->tab[j-1] = 'E' ;
                    ecrireBloc(nom_fichier,i,buf) ;
                    break;
                }
                char sh[MAX_NO_CHARS] ;
                lire_chaine(nom_fichier,buf,&i,&j,TAILLE_EFFECTIVE_ENREG,sh) ;
                lire_chaine(nom_fichier,buf,&i,&j,strToInt(sh),sh);
            }
        }
    // mettre a jour le caractere indiquant le nombre de char logiquements supprime
    affecterEntete(fichier,entete(fichier,ENTETE_NOMBRE_CHAR_SUP),entete(fichier,ENTETE_NOMBRE_CHAR_SUP)+ strToInt(ch)+ TAILLE_EFFECTIVE_ENREG
                                                                            + TAILLE_CHAR_EFFACEMENT_LOGIQUE);
    // fermer(fichier);
    fclose(fichier->fichier) ;
    free(fichier);
    free(buf);
    return true;
    }
    free(fichier);
    free(buf);
    return false;
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
