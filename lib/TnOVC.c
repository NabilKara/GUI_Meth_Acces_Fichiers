#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"TnOVC.h"
#include "utils.h"

bool ouvrir(TnOVC* f,char nom_f[],char mode){
    if(mode == 'N' || mode == 'n'){
        f->fichier = fopen(nom_f,"wb");
        if(f->fichier == NULL) {
        printf("Impossible d'ouvrir le fichier");
        return false;
        }
        affecterEntete(ENTETE_NUMERO_DERNIER_BLOC,1,nom_f); // initialement le dernier bloc est le premier
        affecterEntete(ENTETE_POSLIBRE_DERNIER_BLOC,0,nom_f); // initialement la premiere position est vide
        affecterEntete(ENTETE_NOMBRE_CHAR_SUP,0,nom_f); // initialement aucun caratere n'est supprime
        affecterEntete(ENTETE_NOMBRE_ENREGISTREMENTS,0,nom_f);// fixed.
        FILE *writer = fopen(nom_f,"wb");
        fwrite(&(f->entete),sizeof(Entete),1,writer);
        fclose(writer);
        return true;
    }
    if(mode == 'A' || mode == 'a'){
        f->fichier = fopen(nom_f,"rb+");
        if(f->fichier == NULL) {
            // printf("Impossible d'ouvrir le fichier");
            return false;
        }
        FILE *reader = fopen(nom_f,"rb");
        fread(&(f->entete),sizeof(Entete),1,reader);

        fclose(reader);

        return true;
    }
    return false;
}

bool fermer(TnOVC* f){
    rewind(f->fichier);
    if(fwrite(&(f->entete),sizeof(Entete),1,f->fichier) != sizeof(f->entete)) return false;
    fclose(f->fichier);
    return true;
}

bool lireBloc(char nom_fichier[],int i,Buffer *buf){
    FILE * READER = fopen(nom_fichier,"rb");
    fseek(READER,sizeof(Bloc)*(i-1) + sizeof(Entete),SEEK_SET);
    if(!fread(buf,sizeof(Buffer),1,READER)) return false;
     
     fclose(READER);
     return true;

   
}
bool ecrireBloc(char nom_f[],int i, Buffer* buf) {
    FILE * writer=fopen(nom_f,"rb+");
    fseek(writer,sizeof(Entete)+(i-1)*sizeof(Buffer),SEEK_SET);
    if (!fwrite(buf, sizeof(Buffer), 1, writer)){
        fclose(writer);
        return false;
    }
    fclose(writer);
    return true;
}
int entete(int INDEX,char nom_fichier[]){
    
    FILE * f =fopen(nom_fichier,"rb");
    Entete H;
    
    if (f== NULL) {
        printf("Unable to open the file.\n");
        fclose(f);
        return -111;        //value of null
    }
    fread(&H, sizeof(H), 1, f);
    switch(INDEX){
        case 1: 
                fclose(f);
                return H.numeroDernierBloc;
                break;
        case 2: 
                fclose(f);
                return H.positionLibreDernierBloc;
                break;
                
        case 3:
                fclose(f);
                return H.nbCharSupp;
                break;
        case 4:
                fclose(f);
                return H.nbEnreg;
                break;
        default:
            fclose(f);
            return -222;    //INDEX OverFlow Value
    }
    fclose(f);

}
void affecterEntete(int INDEX,int val,char nom_fichier[]){
    
    Entete H = lireEntete(nom_fichier);
    
    FILE * writer = fopen(nom_fichier,"rb+");
    
    switch(INDEX){
        case 1: 
                H.numeroDernierBloc=val;
                fwrite(&H,sizeof(Entete),1,writer);
                fclose(writer);
                return;
                
        case 2: 
        
                H.positionLibreDernierBloc=val;
                
                fwrite(&H,sizeof(Entete),1,writer);
                fclose(writer);
                
                return;
                
        case 3:
            
                H.nbCharSupp=val;
                
                fwrite(&H,sizeof(Entete),1,writer);
                fclose(writer);
                
                return;
                
        case 4:
                
                H.nbEnreg=val;
                fwrite(&H,sizeof(Entete),1,writer);
                fclose(writer);
                return;
        default:
            printf("INDEX NOT VALID [FROM SET_HEADER]");
            return;
    }
}


/**
* @brief lire une chaine de caracteres depuis un fichier
* @param f pointeur vers un fichier de type TnOVC
* @param buf Buffer en MC 
* @param i pointeur sur le numero du bloc
* @param j pointeur sur la position dans le bloc j
* @param taille taille de la chaine a lire
* @param chaine resultat de la lecture
* @return bool
*/
bool lire_chaine(char nom_fichier[],Buffer* buf,int* i,int* j,int taille,char *ch){
    ch = calloc(taille+1,sizeof(char));
    for (int k = 0; k < taille; k++)
    {
        if(*j <= MAX_NO_CHARS){
            ch[k] = buf->tab[*j];
            (*j)++;
        }else{
            // chevauchement
            if(!lireBloc(nom_fichier,*i,buf)) return false;
            clearString(buf->tab,*j) ;
            (*i)++; // passer au bloc suivant;
            ch[k] = buf->tab[0];
            *j = 1;
        }
    }
    return true;
}

/**
* @brief ecrire une chaine de caracteres dans un fichier
* @param f pointeur vers un fichier de type TnOVC
* @param buf Buffer en MC 
* @param i pointeur sur le numero du bloc
* @param j pointeur sur la position dans le bloc j
* @param taille taille de la chaine a ecrire
* @param chaine la chaine a ecrire
* @return true
* @return false
*/
bool ecrire_chaine(char nom_fichier[],Buffer* buf,int* i,int *j,int taille,char *ch){
    for (int k = 0; k < taille; k++)
    {
        if(*j <= MAX_NO_CHARS){

            buf->tab[*j] = ch[k];
            (*j)++;
        }else{
            // chevauchement:
            if(!ecrireBloc(nom_fichier,*i,buf)) return false;//ecriture du buf dans le bloc
            clearString(buf->tab,*j);
            (*i)++;//incrementation 
            buf->tab[0] = ch[k];
            *j = 1;
        }
    }
    return true;
}

Entete lireEntete(char nom_fichier[]){
    FILE * RH = fopen(nom_fichier,"rb");
    Entete H;
    if (RH== NULL) {
        printf("Unable to open the file.\n");
        return H;
    }
   
    fread(&H,sizeof(Entete),1,RH);
    fclose(RH);

    return H;
}