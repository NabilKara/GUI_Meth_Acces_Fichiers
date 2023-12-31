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

TableIndex* alloc_TabIndex(){
    return (TableIndex*) calloc(sizeof(TableIndex),1);
};

void liberer_TabIndex(TableIndex** t){
    free(*t);
    *t = NULL;
}
 
bool charger_TabIndex(char nom_fich[], TableIndex* t) {
    TOF f;
    Buffer_TOF* buf;
    t->taille = 0;
    if(!ouvrir_TOF(&f,nom_fich,'A')) return false; 

    for (int i = 0; i < entete_TOF(&f,ENTETE_NUMERO_DERNIER_BLOC_TOF); i++)
    {
        lireBloc_TOF(&f,i,buf);
    
        for (int j = 0; j < buf->nbIndex; j++)
        {
            t->tab[t->taille] = buf->tab[j];
            t->taille++;        
        }
        
    }

    fermer_TOF(&f);
    
    return true;
}

bool sauvegarder_TabIndex(char nom_fich[], TableIndex* t) {
    
    TOF f;
    if(!ouvrir_TOF(&f,nom_fich,'N')) return false;
    Buffer_TOF * buf;
    int numBloc = 0;
    int j = 0;
    if (f.fichier == NULL) {
        return false;
    }
    if(t == NULL) return false;

    rewind(f.fichier);
    
    // ecrire les enregistrements de la table d'index dans le fichier 1 par 1
    for (int i = 0; i < t->taille; i++)
    {
        if(j < MAX_INDEX_TOF){
            buf->tab[j] = t->tab[i];
            j++;
        }else{
            buf->nbIndex = MAX_INDEX_TOF; // bloc remppli 
            ecrireBloc_TOF(&f,numBloc,buf); // ecrire le bloc dans le fichier
            numBloc++;
            buf->tab[0] = t->tab[i]; 
            j = 1;
        }
        
    }
    
    buf->nbIndex = j;
    ecrireBloc_TOF(&f,numBloc,buf);

    affecterEntete_TOF(&f,ENTETE_NUMERO_DERNIER_BLOC,numBloc);
    affecterEntete_TOF(&f,ENTETE_NOMBRE_ENREGISTREMENTS,t->taille); 
    fermer_TOF(&f);
    return true;
}