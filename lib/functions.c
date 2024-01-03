#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#include<index.h>
#include<TŌVC.h>
#include<functions.h>
#include<utils.h>

bool rechercher(char nom_fichier[],char cle[TAILLE_CLE],int *i,int *j){
    TŌVC* fichier;
    Buffer* buf;
    if (!ouvrir(fichier,nom_fichier,"a"))
    {
        return false ;
    }
    
    *i = 0;
    *j = 0;
    while (*i < entete(fichier,ENTETE_NUMERO_DERNIER_BLOC) &&  *j != entete(fichier,ENTETE_POSLIBRE_DERNIER_BLOC)) 
    {   
        char chLong[TAILLE_EFFECTIVE_ENREG];
        char chEff[TAILLE_CHAR_EFFACEMENT_LOGIQUE];
        char chCle[TAILLE_CLE];

        lire_chaine(fichier,buf,i,j,TAILLE_EFFECTIVE_ENREG,&chLong);
        lire_chaine(fichier,buf,i,j,TAILLE_CHAR_EFFACEMENT_LOGIQUE,&chEff);
        lire_chaine(fichier,buf,i,j,TAILLE_CLE,&chCle);
        
        if((memcmp(chCle,cle,TAILLE_CLE) == 0) && (memcmp(chEff,'N',TAILLE_CHAR_EFFACEMENT_LOGIQUE) == 0)){
            return true;
        }else{
            *j = *j +  strToInt(chLong) - TAILLE_CLE;
            if(*j > MAX_NO_CHARS){
                // chevauchement
                *j -= MAX_NO_CHARS;
                (*i)++;
                lireBloc(fichier,i,buf);
            }
        }
    }
    fermer(fichier);
    
    return false;
}
bool inserer(char e[],int taille,char nom_fichier[]){
    TŌVC* fichier;
    Buffer* buf;
    char c[TAILLE_EFFECTIVE_ENREG];
    if (!ouvrir(fichier,buf,'a'))
    {
        return false ;
    }
    int i = entete(fichier,ENTETE_NUMERO_DERNIER_BLOC);
    int j = entete(fichier,ENTETE_POSLIBRE_DERNIER_BLOC);
    intToStr(taille,c); 
    
    ecrire_chaine(fichier,buf,&i,&j,TAILLE_EFFECTIVE_ENREG,c);
    ecrire_chaine(fichier,buf,&i,&j,TAILLE_CHAR_EFFACEMENT_LOGIQUE,"N");
    ecrire_chaine(fichier,buf,&i,&j,taille,e);
    
    ecrireBloc(fichier,i,buf);
    if(i != entete(fichier,ENTETE_NUMERO_DERNIER_BLOC)){
        affecterEntete(fichier,ENTETE_NUMERO_DERNIER_BLOC,i);
        // mettre a jour le numero du dernier bloc
    }

    affecterEntete(fichier,ENTETE_POSLIBRE_DERNIER_BLOC,j);
    // mettre a jour la premiere position libre dans le dernier bloc

    fermer(fichier);
}
bool suppression_logique(char cle[TAILLE_CLE], char nom_fichier[]){
    TŌVC* fichier;
    Buffer* buf;
    char *ch[TAILLE_EFFECTIVE_ENREG];
    int i,j;
    if(rechercher(nom_fichier,cle,&i,&j)){
        if (!ouvrir(fichier,nom_fichier,'a'))
        {
            return false ;
        }
        lireBloc(fichier,i,buf);
        lire_chaine(fichier,buf,&i,&j,TAILLE_EFFECTIVE_ENREG,ch);
        if(j <= MAX_NO_CHARS){
            buf->tab[j] = "E";  // positioner le caractere d'effacement logique
        }else{
            //chevauchement
            i++;
            lireBloc(fichier,i,buf);
            buf->tab[1] = "E";
        }
    ecrireBloc(fichier,i,buf);
    // mettre a jour le caractere indiquant le nombre de char logiquements supprime
    affecterEntete(fichier,entete(fichier,ENTETE_NOMBRE_CHAR_SUP),entete(fichier,ENTETE_NOMBRE_CHAR_SUP)+ strToInt(ch)+ TAILLE_EFFECTIVE_ENREG
                                                                            + TAILLE_CHAR_EFFACEMENT_LOGIQUE);
    fermer(fichier);
    }
}

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

bool creerTableIndex(char nom_fich[])
{
    TŌVC *f;
    if (!ouvrir(f,nom_fich,'A'))
    {
        printf("Erreur de l'ouverture de fichier");
        exit(1) ;
    }
    tabIndex = alloc_TabIndex();
    Buffer *buf ;
    DataIndex data;
    char position[TAILLE_EFFECTIVE_ENREG] ;
    int i = 1 ;
    int j = 0 ;
    while (i <= f->entete.numeroDernierBloc)
    {
        if(!lireBloc(f,i,buf))
        {
            return false;
        }
        strncpy(data.cle,buf,TAILLE_CLE);
        data.numBloc = i ;
        recupererStr(buf,TAILLE_CLE+1,position) ;
        data.posBloc = TAILLE_CLE + 1 + TAILLE_EFFECTIVE_ENREG + strToInt(position) ;
        if(!updateTableIndex(data,tabIndex,'A'))
        {
            return false;
        }
        i++ ;
    }

    
}
