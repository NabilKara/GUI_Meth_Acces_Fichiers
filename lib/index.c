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
bool lireBloc_TOF(TOF* f,int i,Buffer *buf){

}
bool ecrireBloc_TOF(TOF* f,int i,Buffer* buf){

}

bool allouerBloc_TOF(TOF* f){

}

TableIndex* alloc_TabIndex(); 
void liberer_TabIndex(TableIndex* t); 
bool charger_TabIndex(char nom_fich[],TableIndex* t); 
bool sauvegarder_TabIndex(char nom_fich[],TableIndex* t);