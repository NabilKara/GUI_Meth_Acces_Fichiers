#include<stdio.h>
#include<stdlib.h>
#include<TŌVC.h>

void ouvrir(TŌVC* f,char nom_f[],char mode){
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
            printf("Impossible d'ouvrir le fichier");
            return;    
        }
        fread(&(f->entete),sizeof(Entete),1,f->fichier);
        rewind(f);
    }
}

void fermer(TŌVC* f){
    rewind(f);
    fwrite(&(f->entete),sizeof(Entete),1,f->fichier);
    fclose(f);
}

void lireBloc(TŌVC* f,int i,Buffer *buf){
    fseek(f->fichier,sizeof(Bloc)*(i-1) + sizeof(Entete),SEEK_SET);
    fread(buf,sizeof(Buffer),1,f->fichier); 
}
void ecrireBloc(TŌVC* f,int i,Buffer* buf){
    fseek(f->fichier,sizeof(Bloc)*(i-1) + sizeof(Entete),SEEK_SET);
    fwrite(buf,sizeof(Buffer),1,f->fichier);
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
    default:
        printf("Erreur lors du chargement.\n\tCaracteristique n'existe pas");
        return -1;
    }
}
void affecterEntete(TŌVC* f,int i,int val){
    switch (i)
    {
    case ENTETE_NUMERO_DERNIER_BLOC:
        f->entete.numeroDernierBloc = val;
        break;
    case ENTETE_POSLIBRE_DERNIER_BLOC:
        f->entete.positionLibreDernierBloc = val;
        break;
    case ENTETE_NOMBRE_CHAR_SUP:
        f->entete.nbCharSupp = val;
    default:
        printf("Erreur lors du chargement.\n\tCaracteristique n'existe pas");
        break;
    }
}
void allouerBloc(TŌVC* f){
    int numDernierBloc = entete(f,ENTETE_NUMERO_DERNIER_BLOC)+1;
    affecterEntete(f,ENTETE_NUMERO_DERNIER_BLOC,numDernierBloc);
    return numDernierBloc;
}

void lire_chaine(TŌVC* f,Buffer* buf,int* i,int* j,int taille,char *ch[]){
    *ch = calloc(taille+1,sizeof(char));
    for (int k = 0; k < taille; k++)
    {
        if(*j <= MAX_NO_CHARS){
            (*ch)[k] = buf->tab[*j];
            j++;
        }else{
            // chevauchement
            i++; // bloc-svt;
            lireBloc(f,i,buf);
            (*ch)[k] = buf->tab[1];
            j = 2;
        }
    }
}
void ecrire_chaine(TŌVC* f,Buffer* buf,int* i,int *j,int taille,char ch[]){
    for (int k = 0; k < taille; k++)
    {
        if(j <= MAX_NO_CHARS){
            buf->tab[*j] = ch[k];
            j++; 
        }else{
            // chevauchement
            i++;
            lireBloc(f,i,buf);
            buf->tab[1] = ch[k];
            *j = 2;
        }
    }
}
void afficher_entete(TŌVC* f){}
void afficher_bloc(TŌVC* f,int i){}
void afficher_fichier(TŌVC* f){}