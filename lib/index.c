#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "/home/nabilkara/Desktop/S3/SFSD/GUI_meth_acees_fichiers/lib/index.h"


 // fonctions du modele

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
    if(mode == 'N' || mode == 'n'){

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
    }else if(mode == 'A' || mode == 'a'){
        f->fichier = fopen(nom_f,"rb+");
        if(f->fichier == NULL) return false;

        if(!fread(&(f->entete),sizeof(f->entete),1,f->fichier)) return false;
        rewind(f->fichier);
    }
    return true;
}

bool fermer_TOF(TOF* f){
    rewind(f->fichier);
    if(fwrite(&(f->entete),sizeof(Entete_TOF),1,f->fichier) != 1) return false;
    fclose(f->fichier);
    return true;
}

bool lireBloc_TOF(TOF* f,int i,Buffer_TOF *buf){
    fseek(f->fichier,sizeof(Bloc_TOF)*(i-1) + sizeof(Entete_TOF),SEEK_SET);
    if(!fread(buf,sizeof(Buffer_TOF),1,f->fichier)) return false; 
    return true;
}

bool ecrireBloc_TOF(TOF* f,int i,Buffer_TOF* buf){
    fseek(f->fichier,sizeof(Bloc_TOF)*(i-1) + sizeof(Entete_TOF),SEEK_SET);
    if(fwrite(buf,sizeof(Buffer_TOF),1,f->fichier) != sizeof(Buffer_TOF) != 1) return false;
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

/**
 * @brief allouer la table d'index en MC
 * 
 * @return TableIndex* 
 */
TableIndex* alloc_TabIndex(){
    return (TableIndex*) calloc(sizeof(TableIndex),1);
};

/**
 * @brief Liberer la table d'index de la MC
 * 
 * @param t La table d'index 
 */
void liberer_TabIndex(TableIndex** t){
    free(*t);
    *t = NULL;
}

/**
 * @brief charger la table d'index depuis un fichier TOF
 * 
 * @param nom_fich nom du fichier
 * @param t La table d'index
 * @return true 
 * @return false 
 */
bool charger_TabIndex(char nom_fich[], TableIndex* t) {
    TOF f;
    Buffer_TOF buf;
    t->taille = 0;
    if(!ouvrir_TOF(&f,nom_fich,'A')) return false; 

    for (int i = 1; i <= entete_TOF(&f,ENTETE_NUMERO_DERNIER_BLOC_TOF); i++)
    {
        lireBloc_TOF(&f,i,&buf);
    
        for (int j = 0; j < buf.nbIndex; j++)
        {
            t->tab[t->taille] = buf.tab[j];
            t->taille++;        
        }
        
    }

    fermer_TOF(&f);
    
    return true;
}

/**
 * @brief Sauvegarder la table d'index dans un fichier TOF
 * 
 * @param nom_fich nom du fichier
 * @param t La table d'index
 * @return true 
 * @return false 
 */
bool sauvegarder_TabIndex(char nom_fich[], TableIndex* t) {
    
    TOF f;
    Buffer_TOF buf;
    int numBloc = 1;
    int j = 0;
    if(!ouvrir_TOF(&f,nom_fich,'N')) return false;
    
    // if (f.fichier== NULL) {
    //     return false;
    // }
    // if(t == NULL) return false;
    
    // rewind(f.fichier);
    
    // ecrire les enregistrements de la table d'index dans le fichier 1 par 1
    for (int i = 0; i < t->taille; i++)
    {
        if(j < MAX_INDEX_TOF){
            buf.tab[j] = t->tab[i];
            j++;
        }else{
            buf.nbIndex = MAX_INDEX_TOF; // bloc rempli 
            ecrireBloc_TOF(&f,numBloc,&buf); // ecrire le bloc dans le fichier
            numBloc++;
            buf.tab[0] = t->tab[i]; 
            j = 1;
        }
    }
    
    buf.nbIndex = j;
    ecrireBloc_TOF(&f,numBloc,&buf);

    affecterEntete_TOF(&f,ENTETE_NUMERO_DERNIER_BLOC_TOF,numBloc);
    affecterEntete_TOF(&f,ENTETE_NOMBRE_ENREGISTREMENTS_TOF,t->taille); 
    fermer_TOF(&f);
    return true;
}

/**
 * @brief Mettre a jour la table d'index (insertion , suppression)
 * 
 * @param dataInd L'index de l'enregistrement dans la table d'index
 * @param t La table d'index
 * @param action S pour supprimer et A pour inserer
 * @return true 
 * @return false 
 */
bool updateTableIndex(DataIndex dataInd, TableIndex* t, char action)
{

    if (action == 'S' || action == 's')
    {
        bool trouve = false ;
        int i = 0 ; 
        int j = t->taille ;
        int m ;
        while (i < j && !trouve)
        {
            m = (i+j) / 2 ;
            if (strcmp(t->tab[m].cle,dataInd.cle) == 0)
            {
                trouve = true ;
            }
            else if (strcmp(t->tab[m].cle,dataInd.cle) < 0)
                 {
                    i = m + 1 ;
                 }
                 else
                 {
                    j = m ;
                 }
                 
            
        }
        if (trouve == true)
        {
            for (int j = i; j < t->taille - 1; j++)
            {
                t->tab[j] = t->tab[j+1] ;
            }
        }
        t->taille-- ;//decrementer la taille 
        return trouve ;
        
    }
    else if (action == 'A' || action == 'a')
        {
            if (t->taille == MAX_INDEX)
            {
                return false ;//la table est plaine
            }
            // recherche dicho 
            int debut , fin , milieu ;
            debut = 0 ;
            fin = t->taille ;
            
            while ((fin - debut) != 1)
            {
                milieu = (debut + fin) / 2 ;
                if(strcmp(t->tab[milieu].cle,dataInd.cle) == 0)
                {
                    return false ;// la valeur deja existe
                }
                else if (strcmp(t->tab[milieu].cle,dataInd.cle) < 0) // t->tab[milieu] < cle
                     {
                        debut = milieu ;
                     }
                     else
                     {
                        fin = milieu ;
                     }                
                
            }


            for (int i = t->taille ; i >= fin ; i--)
            {
                t->tab[i]= t->tab[i-1] ;
            }

            t->tab[debut+1] = dataInd ;
            t->taille++ ;

            return true; //succes Ajout 
            

        }
        else
        {
            return false; //action != 'A' et action != 'S'
        }   
}