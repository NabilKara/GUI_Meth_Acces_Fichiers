#include<stdlib.h>
#include<stdio.h>
#include<index.h>
#include<TŌVC.h>

int entete_TOF(TOF* f,int i){
    switch (i)
    {
    case ENTETE_NUMERO_DERNIER_BLOC_TOF:
        return f->entete.numDernierBloc;
    case ENTETE_NOMBRE_ENREGISTREMENTS_TOF:
        return f->entete.nbEnreg;
    default:
        return -1;
    }
}
bool affecterEntete_TOF(TOF* f,int i,int val){
    switch (i)
    {
    case ENTETE_NUMERO_DERNIER_BLOC_TOF:
        f->entete.numDernierBloc = val;
        break;
    case ENTETE_NOMBRE_ENREGISTREMENTS_TOF:
        f->entete.nbEnreg = val;
    default:
        return false;
    }
    return true;
}
bool ouvrir_TOF(TOF* f,char nom_f[],char mode){
    if(mode == 'N'){

    f->fichier = fopen(nom_f,"wb+");
    
    if(f->fichier == NULL){
        return false;
    }

    // initialiser l'entete

    affecterEntete_TOF(f,ENTETE_NUMERO_DERNIER_BLOC_TOF,0);
    affecterEntete_TOF(f,ENTETE_NOMBRE_ENREGISTREMENTS_TOF,0);
    
    // ecrire l'entete dans le fichier
    if(!fwrite(&(f->entete),sizeof(f->entete),1,f->fichier)) return false;
    rewind(f->fichier);
    }else{
        f->fichier = fopen(nom_f,"rb+");
        if(f->fichier == NULL) return false;

        if(!fread(&(f->entete),sizeof(f->entete),1,f->fichier)) return false;
        rewind(f->fichier);
    }
    return true;
}
bool fermer_TOF(TOF* f){
    rewind(f->fichier);
    if(!fwrite(&(f->entete),sizeof(f->entete),1,f->fichier)) return false;
    fclose(f->fichier);
}
bool lireBloc_TOF(TOF* f,int i,Buffer_TOF *buf){
    fseek(f->fichier,sizeof(Bloc_TOF)*(i-1) + sizeof(Entete_TOF),SEEK_SET);
    if(!fread(buf,sizeof(Buffer_TOF),1,f->fichier)) return false; 
    return true;
}
bool ecrireBloc_TOF(TOF* f,int i,Buffer_TOF* buf){
    fseek(f->fichier,sizeof(Bloc_TOF)*(i-1) + sizeof(Entete_TOF),SEEK_SET);
    if(fwrite(buf,sizeof(Buffer_TOF),1,f->fichier) != sizeof(Buffer_TOF)) return false;
    return true;
}

int allouerBloc_TOF(TOF* f){
    int numDernierBloc = entete_TOF(f,ENTETE_NUMERO_DERNIER_BLOC_TOF) + 1 ;
    if (!affecterEntete_TOF(f,ENTETE_NUMERO_DERNIER_BLOC_TOF,numDernierBloc))
    {
        return -1;
    }
    return numDernierBloc;
}

TableIndex* alloc_TabIndex(); 
void liberer_TabIndex(TableIndex* t);
 
bool charger_TabIndex(char nom_fich[], TableIndex* t) {
    FILE* fichier = fopen(nom_fich, "rb");
    if (fichier == NULL) {
        return false;
    }

    fread(t, sizeof(TableIndex), 1, fichier);
    fclose(fichier);
    return true;
}

bool sauvegarder_TabIndex(char nom_fich[], TableIndex* t) {
    FILE* fichier = fopen(nom_fich, "wb");
    if (fichier == NULL) {
        return false;
    }

    fwrite(t, sizeof(TableIndex), 1, fichier);
    fclose(fichier);
    return true;
}