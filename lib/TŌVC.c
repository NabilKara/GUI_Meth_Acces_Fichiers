#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<TŌVC.h>

bool ouvrir(TŌVC* f,char nom_f[],char mode){
    if(mode == 'N' || mode == 'n'){
        f->fichier = fopen(nom_f,"wb+");    
        if(f->fichier == NULL) {
        printf("Impossible d'ouvrir le fichier");
        return;
        } 
        affecterEntete(f,ENTETE_NUMERO_DERNIER_BLOC,0); // initialement le dernier bloc est le premier
        affecterEntete(f,ENTETE_POSLIBRE_DERNIER_BLOC,0); // initialement la premiere position est vide
        affecterEntete(f,ENTETE_NOMBRE_CHAR_SUP,0); // initialement aucun caratere n'est supprime
        fwrite(&(f->entete),sizeof(Entete),1,f->fichier);
        rewind(f->fichier);
    }else if(mode == 'A'){
        f->fichier = fopen(nom_f,"rb+");
        if(f->fichier == NULL) {
            // printf("Impossible d'ouvrir le fichier");
            return false;    
        }
        fread(&(f->entete),sizeof(f->entete),1,f->fichier);
        rewind(f->fichier);
    }
    return true;
}

bool fermer(TŌVC* f){
    rewind(f->fichier);
    if(fwrite(&(f->entete),sizeof(f->entete),1,f->fichier) != sizeof(f->entete)) return false;
    fclose(f->fichier);
    return true;
}

bool lireBloc(TŌVC* f,int i,Buffer *buf){
    fseek(f->fichier,sizeof(Bloc)*(i-1) + sizeof(Entete),SEEK_SET);
    if(!fread(buf,sizeof(Buffer),1,f->fichier)) return false; 
    return true;
}
bool ecrireBloc(TŌVC* f,int i,Buffer* buf){
    fseek(f->fichier,sizeof(Bloc)*(i-1) + sizeof(Entete),SEEK_SET);
    if(fwrite(buf,sizeof(Buffer),1,f->fichier) != sizeof(Buffer)) return false;
    return true;
}
int entete(TŌVC* f,int i){
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
bool affecterEntete(TŌVC* f,int i,int val){
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
        break;    
    default:
       // printf("Erreur lors du chargement.\n\tCaracteristique n'existe pas");
        return false;
    }
}
int allouerBloc(TŌVC* f){
    int numDernierBloc = entete(f,ENTETE_NUMERO_DERNIER_BLOC)+1;
    if(!affecterEntete(f,ENTETE_NUMERO_DERNIER_BLOC,numDernierBloc)) return -1;
    return numDernierBloc;
}

bool lire_chaine(TŌVC* f,Buffer* buf,int* i,int* j,int taille,char *ch[]){
    *ch = calloc(taille+1,sizeof(char));
    for (int k = 0; k < taille; k++)
    {
        if(*j <= MAX_NO_CHARS){
            (*ch)[k] = buf->tab[*j];
            (*j)++;
        }else{
            // chevauchement
            (*i)++; // passer au bloc suivant;
            if(!lireBloc(f,*i,buf)) return false;
            (*ch)[k] = buf->tab[1];
            *j = 2;
        }
    }
    return true;
}
bool ecrire_chaine(TŌVC* f,Buffer* buf,int* i,int *j,int taille,char ch[]){
    for (int k = 0; k < taille; k++)
    {
        if(*j <= MAX_NO_CHARS){
            buf->tab[*j] = ch[k];
            (*j)++; 
        }else{
            // chevauchement
            (*i)++;
            if(!lireBloc(f,*i,buf)) return false;
            buf->tab[1] = ch[k];
            *j = 2;
        }
    }
    return true;
}