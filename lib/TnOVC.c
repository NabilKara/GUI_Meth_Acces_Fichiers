#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"TnOVC.h"

bool ouvrir(TnOVC* f,char nom_f[],char mode){
    if(mode == 'N' || mode == 'n'){
        f->fichier = fopen(nom_f,"wb");
        if(f->fichier == NULL) {
        printf("Impossible d'ouvrir le fichier");
        return false;
        }
        affecterEntete(f,ENTETE_NUMERO_DERNIER_BLOC,1); // initialement le dernier bloc est le premier
        affecterEntete(f,ENTETE_POSLIBRE_DERNIER_BLOC,0); // initialement la premiere position est vide
        affecterEntete(f,ENTETE_NOMBRE_CHAR_SUP,0); // initialement aucun caratere n'est supprime
        affecterEntete(f,ENTETE_NOMBRE_ENREGISTREMENTS,0);// fixed.
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
    if(fwrite(&(f->entete),sizeof(Entete),1,f->fichier) != 1) return false;
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

int entete(TnOVC* f,int i){
    switch (i)
    {
    case ENTETE_NUMERO_DERNIER_BLOC:
        return f->entete.numeroDernierBloc;
    case ENTETE_POSLIBRE_DERNIER_BLOC:
        return f->entete.positionLibreDernierBloc;
    case ENTETE_NOMBRE_CHAR_SUP:
        return f->entete.nbCharSupp;
    case ENTETE_NOMBRE_ENREGISTREMENTS:
        return f->entete.nbEnreg;
    default:
        printf("Erreur lors du chargement.\n\tCaracteristique n'existe pas");
        return -1;
    }
}
bool affecterEntete(TnOVC* f,int i,int val){
    switch (i)
    {
    case ENTETE_NUMERO_DERNIER_BLOC:
        f->entete.numeroDernierBloc = val;
        return true;
    case ENTETE_POSLIBRE_DERNIER_BLOC:
        f->entete.positionLibreDernierBloc = val;
        return true;
    case ENTETE_NOMBRE_CHAR_SUP:
        f->entete.nbCharSupp = val;
        return true;
    case ENTETE_NOMBRE_ENREGISTREMENTS:
        f->entete.nbEnreg = val;
        return true;
        break;    
    default:
       // printf("Erreur lors du chargement.\n\tCaracteristique n'existe pas");
        return false;
    }
}
int allouerBloc(TnOVC* f){
    int numDernierBloc = entete(f,ENTETE_NUMERO_DERNIER_BLOC)+1;
    if(!affecterEntete(f,ENTETE_NUMERO_DERNIER_BLOC,numDernierBloc)) return -1;
    return numDernierBloc;
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
bool lire_chaine(char nom_fichier[],Buffer* buf,int* i,int* j,int taille,char ch[]){
    ch[taille] = '\0' ;
    for (int k = 0; k < taille; k++)
    {
        if(*j <= MAX_NO_CHARS){
            ch[k] = buf->tab[*j];
            (*j)++;
        }else{
            // chevauchement
            (*i)++; // passer au bloc suivant;
            clearString(buf->tab,*j);
            if (!lireBloc(nom_fichier,*i, buf)) return false;
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
            if(!ecrireBloc(nom_fichier,*i,buf)) return false;//ecriture du buf
            clearString(buf->tab,*j);
            (*i)++;//incrementation 
            buf->tab[0] = ch[k];
            *j = 1;
        }
    }
    return true;
}
Entete readHeader(){
    FILE * RH = fopen("projet.bin","rb");
    Entete H;
    if (RH== NULL) {
        printf("Unable to open the file.\n");
        return H;
    }
   
    fread(&H,sizeof(Entete),1,RH);
    fclose(RH);

    return H;
}
void setHeader(int INDEX,int val){
    
    Entete H = readHeader();
    
    FILE * writer = fopen("projet.bin","rb+");
    
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
int getHeader(int INDEX){
    
    FILE * f =fopen("projet.bin","rb");
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