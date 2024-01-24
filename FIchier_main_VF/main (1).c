#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
/*---Constantes---*/
#define MAX_NO_CHARS 99 //le nombre maximal des caracteres dans un bloc

// Numero des entetes
#define ENTETE_NUMERO_DERNIER_BLOC 1
#define ENTETE_POSLIBRE_DERNIER_BLOC 2
#define ENTETE_NOMBRE_CHAR_SUP 3
#define ENTETE_NOMBRE_ENREGISTREMENTS 4

// representation d'un enregistrement
#define TAILLE_EFFECTIVE_ENREG 3 // la taille effective de la donne dans l'enregistrement est sur 3 octects
#define TAILLE_CHAR_EFFACEMENT_LOGIQUE 1 // "E": pour efface , "N" pour: non efface
#define TAILLE_CLE 20
// n le reste

/*--Structures--*/

typedef struct
{
    char tab[MAX_NO_CHARS + 1];
    int nb;
}Bloc, Buffer;

typedef struct
{
    int numeroDernierBloc;
    int positionLibreDernierBloc;
    int nbCharSupp;
    int nbEnreg;
}Entete;
typedef struct
{
    Entete entete;
    FILE* fichier;
}TOVC;

//======================//
char nom_fichier_originale[] = "projet.bin" ;
char fichier_index[] = "index.bin";
// declarations des constantes
#define MAX_INDEX 1000// taille de la table d'index
// max index est a revoir plus tard , je sais pas encore comment recuperer le nombre de cle dans un fichier
#define MAX_INDEX_TOF 4 // taille du buffer du fichier d'index

#define MAX_CHARS_TOF 104
// Numeros des entetes

#define ENTETE_NUMERO_DERNIER_BLOC_TOF 1
#define ENTETE_NOMBRE_ENREGISTREMENTS_TOF 2

typedef struct
{
    char cle[TAILLE_CLE];
    int numBloc;
    int posBloc;
}DataIndex;

typedef struct
{
    char index[MAX_CHARS_TOF + 1] ;
}Bloc_TOF,Buffer_TOF;

typedef struct
{
    DataIndex tab[MAX_INDEX];
    int taille;
}TablIndex;

typedef struct
{
    int numDernierBloc;
    int nbEnreg; // nombre d'enregistrements inseres
}Entete_TOF;

typedef struct
{
    Entete_TOF entete;
    FILE* fichier;
}TOF;
// variables globales
TablIndex *tabIndex;  // table d'index generale
bool tabIndexExis; // verifier si la table d'index existe


typedef struct IndexEntete
{
    int nbElement ;
}IndexEntete;

//=============var globales
IndexEntete index_Entete ;

typedef struct
{
    int i , j , cle ;
}Dataindex;
// IndexEntete lire_index_Entete()
// {
//     FILE *E = fopen(nom_fichier_index,"rb");
//     if(E == NULL)
//     {
//         printf("ouverture impossible");
//         return E;
//     }
    
// }
void setIndexEntete(int i)
{
    index_Entete.nbElement = i ; 
}

int getIndexEntete()
{
    return index_Entete.nbElement ;
}

Dataindex TableIndex[100] ;

void trierTableIndex()
{
    int n = getIndexEntete() ;
    Dataindex d ;
    for(int i=0;i<n-1;i++)
    {
        for(int j=i+1;j<n;j++)
        {
            if(TableIndex[i].cle > TableIndex[j].cle)
            {
                d = TableIndex[i] ;
                TableIndex[i] = TableIndex[j] ;
                TableIndex[j] = d ;
            }
        }
    }
}


// void afficherTable()
// {
//     for(int i=0;i<getIndexEntete();i++)
//     {
//         printf("cle = %d , i = %d , j = %d\n",TableIndex[i].cle,TableIndex[i].i,TableIndex[i].j) ;
//     }
// }

//=====new==========
Entete readHeader();
void setHeader(int INDEX, int val);
int getHeader(int INDEX);

Entete_TOF readHeaderTOF();
void setHeader_TOF(int INDEX, int val);
int getHeader_TOF(int INDEX);
//============================

bool affecterEntete(TOVC* f,int i,int val){
    if(f==NULL){
        return false;
    }
    switch (i)
    {
    case 1:
        ((*f).entete).numeroDernierBloc = val;
        return true;
    case 2:
        (f->entete).positionLibreDernierBloc = val;
        return true;
    case 3:
        (f->entete).nbCharSupp = val;
        return true;
    case 4:
        (f->entete).nbEnreg = val;
        return true;
    default:
       // printf("Erreur lors du chargement.\n\tCaracteristique n'existe pas");
        return false;
    }
}

bool ouvrir(TOVC* f,char nom_f[],char mode){
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

int entete(TOVC* f,int i){
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

void clearString(char *str, size_t size) {
    if (str == NULL || size == 0) {
        // Handle invalid input
        return;
    }

    // Fill the string with null characters
    memset(str, '\0', size);
}

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

int strToInt(char ch[])
{
    return atoi(ch);
}
int nbPos(int n)
{
    int pos = 1;

    while (n / 10 != 0)
    {
        pos++;
        n /= 10;
    }
    return pos;
}

void intToStr(int n, char chaine[], int taille)
{
    const int ZEROS = taille - nbPos(n);
    if (ZEROS < 0)
    {
        printf("[ERROR] la taille de l'entier a convertir vers une chaine de caractere est plus grande que la taille de la chaine !!\n");
        strcpy(chaine, "");
        return;
    }

    char *temp_str = calloc(taille + 1, sizeof(char));
    sprintf(temp_str, "%d", n);

    for (int i = 0; i < ZEROS; i++)
        chaine[i] = '0';
    chaine[ZEROS] = '\0';

    strcat(chaine, temp_str);
    free(temp_str);
}
bool inserer(char e[],int taille,char nom_fichier[],int cle){
    TOVC* fichier=malloc(sizeof(TOVC));
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
    
    TableIndex[getIndexEntete()].cle = cle ;
    
    
    ecrire_chaine(nom_fichier,buf,&i,&j,TAILLE_CLE,id);
    ecrire_chaine(nom_fichier,buf,&i,&j,TAILLE_CHAR_EFFACEMENT_LOGIQUE,"N");
    ecrire_chaine(nom_fichier,buf,&i,&j,TAILLE_EFFECTIVE_ENREG,c);
    
    TableIndex[getIndexEntete()].i = i ;
    TableIndex[getIndexEntete()].j = j ;
    
    ecrire_chaine(nom_fichier,buf,&i,&j,taille,e);
    
    
    
    setIndexEntete(getIndexEntete() + 1);
    trierTableIndex() ;
    
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

bool rechercher(char nom_fichier[],char cle[TAILLE_CLE],int *i,int *j){
    TOVC* fichier = malloc(sizeof(TOVC));
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

bool suppression_logique(char cle[TAILLE_CLE], char nom_fichier[]){
    TOVC* fichier = malloc(sizeof(TOVC));
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




bool ouvrir_TOF(TOF* f,char nom_f[],char mode){
    if(mode == 'N' || mode == 'n'){
        f->fichier = fopen(nom_f,"wb");
        if(f->fichier == NULL) {
        printf("Impossible d'ouvrir le fichier");
        return false;
        }
        setHeader_TOF(1,1) ;
        setHeader_TOF(2,0) ;
        FILE *writer = fopen(nom_f,"wb");
        fwrite(&(f->entete),sizeof(Entete_TOF),1,writer);
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
        fread(&(f->entete),sizeof(Entete_TOF),1,reader);

        fclose(reader);

        return true;
    }
    return false;
}

bool lireBloc_TOF(char nom_fichier[],int i,Buffer_TOF *buf){
    FILE * READER = fopen(nom_fichier,"rb");
    fseek(READER,sizeof(Bloc_TOF)*(i-1) + sizeof(Entete_TOF),SEEK_SET);
    if(!fread(buf,sizeof(Buffer_TOF),1,READER)) return false;
     
     fclose(READER);
     return true;

   
}

bool ecrireBloc_TOF(char nom_f[],int i, Buffer_TOF* buf) {
    FILE * writer=fopen(nom_f,"rb+");
    fseek(writer,sizeof(Entete_TOF)+(i-1)*sizeof(Buffer_TOF),SEEK_SET);
    if (!fwrite(buf, sizeof(Buffer_TOF), 1, writer)){
        fclose(writer);
        return false;
    }
    fclose(writer);
    return true;
}

bool lire_chaine_TOF(char nom_fichier[],Buffer_TOF* buf,int* i,int* j,int taille,char ch[]){
    ch[taille] = '\0' ;
    for (int k = 0; k < taille; k++)
    {
        if(*j <= MAX_CHARS_TOF){
            ch[k] = buf->index[*j];
            (*j)++;
        }else
        {
            return false;
        }
    }
    return true;
}

bool ecrire_chaine_TOF(char nom_fichier[],Buffer_TOF* buf,int* i,int *j,int taille,char *ch){
    for (int k = 0; k < taille; k++)
    {
        if(*j <= MAX_CHARS_TOF){

            buf->index[*j] = ch[k];
            (*j)++;
        }else{
            return false;
        }
    }
    return true;
}

bool Charger_TOF()
{
    TOF *f = malloc(sizeof(TOF)) ;
    Buffer_TOF *buf = malloc(sizeof(Buffer_TOF)) ;
    
    lireBloc_TOF(fichier_index,getHeader_TOF(1),buf);
    
    if(!ouvrir_TOF(f,fichier_index,'N'))
    {
        printf("impossible d'ouvrir le fichier");
        return false;
    }
    
    char cle[TAILLE_CLE] ;
    char I[TAILLE_EFFECTIVE_ENREG] ;
    char J[TAILLE_EFFECTIVE_ENREG] ;
    
    int i = 0,j;
    for(int k = 0;k < getIndexEntete();k++)
    {
        j = 0 ;
        i = i + 1 ;
        intToStr(TableIndex[k].cle,cle,TAILLE_CLE) ;
        intToStr(TableIndex[k].i,I,TAILLE_EFFECTIVE_ENREG);
        intToStr(TableIndex[k].j,J,TAILLE_EFFECTIVE_ENREG);
        
        ecrire_chaine_TOF(fichier_index,buf,&i,&j,TAILLE_CLE,cle) ;
        ecrire_chaine_TOF(fichier_index,buf,&i,&j,TAILLE_CLE,I) ;
        ecrire_chaine_TOF(fichier_index,buf,&i,&j,TAILLE_CLE,J) ;
        
        ecrireBloc_TOF(fichier_index,i,buf) ;
    }
    
    setHeader_TOF(1,i) ;
    setHeader_TOF(2,i*4) ;
    
    return true;
    
}



void about(){
    printf("\n\t\t=============================================================================\n");
    printf("\t\t\t\t\t   projet SFSD \n");
    printf("\t\t\tGroupe[9] : SIFI Tarek Mazigh   KARA Nabil   BENALIA Mohammed\n");
    printf("\t\t=============================================================================\n");
}

int recupererId()
{
    int i ;
    do{
        printf("id = ");
        scanf("%d",&i);
    }while(i<=0) ;
    return i;
}

void recupererChaine(char ch[])
{
    printf("la donner : ");
    scanf("%s",ch);
}

void affichage(){
    printf("\t\t\t1 - inserer un etudient\n");
    printf("\t\t\t2 - afficher un etudiant\n");
    printf("\t\t\t3 - afficher tout les etudiants\n");
    printf("\t\t\t4 - supprimer un etudiant\n");
    printf("\t\t\t5 - exit\n");
}

int choix(){
    int i ;
    do
    {
        affichage();
        printf("choix = ");
        scanf("%d",&i);
    }while(i<=0 || i>5);
    return i ;
}

void afficherDonner(char nom_fichier[],char cle[TAILLE_CLE],int *i,int *j){
    TOVC* fichier = malloc(sizeof(TOVC));
    Buffer* buf = malloc(sizeof(Buffer));
    
    if (!ouvrir(fichier,nom_fichier,'a'))
    {
        return ;
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
            printf("\n -la cle : %s",cle);
            printf("\n -Donner : %s\n\n",data);
            return ;
        }
            
        
    }
    fclose(fichier->fichier);
    free(fichier);
    free(buf);
    return ;
}

void afficherToutDonner(char nom_fichier[],int *i,int *j){
    TOVC* fichier = malloc(sizeof(TOVC));
    Buffer* buf = malloc(sizeof(Buffer));
    
    if (!ouvrir(fichier,nom_fichier,'a'))
    {
        return ;
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
        

        
        if(y == z)
        {
            intToStr(x,chCle,TAILLE_CLE);
            printf("\n -la cle : %s",chCle);
            printf("\n -Donner : %s\n\n",data);
        }
            
        
    }
    fclose(fichier->fichier);
    free(fichier);
    free(buf);
    return ;
}



void fonctions(char nom_fichier_originale[])
{
    int id ;
    char cle[TAILLE_CLE] ;
    char ch[MAX_NO_CHARS] ;
    debut : 
    switch(choix())
    {
        case 1 : 
                 id = recupererId() ;
                 recupererChaine(ch) ;
                 inserer(ch,strlen(ch),nom_fichier_originale,id);
                 Charger_TOF() ;
                 goto debut ;
                 break;
        case 2 :
                 id = recupererId() ;
                 intToStr(id,cle,TAILLE_CLE);
                 int i , j ;
                 if(rechercher(nom_fichier_originale,cle,&i,&j))
                 {
                    afficherDonner(nom_fichier_originale,cle,&i,&j) ;
                 }
                 else
                 {
                     printf("n'existe pas\n");
                 }
                 goto debut;
                 break ;
        case 4 : id = recupererId() ;
                 intToStr(id,cle,TAILLE_CLE);
                 if(suppression_logique(cle,nom_fichier_originale))
                 {
                    printf("la cle %s a ete bien supprimer\n\n",cle);
                 }
                 else
                 {
                    printf("false id ou deja supprimer\n") ;
                 }
                 Charger_TOF() ;
                 goto debut ;
                 break ;
                 
        case 3 : afficherToutDonner(nom_fichier_originale,&i,&j) ;
                 goto debut ;
                 break ;
        default : return ;
    }
}


int main()
{
    TOVC *fichier_originale = malloc(sizeof(TOVC)) ;
    TOF *f = malloc(sizeof(TOF)) ;
    int existe ;
    about();
    printf("\nest ce que le fichier existe deja ?? : ");
    
    scanf("%d",&existe);
    if(existe == 0)
    {
        if(!ouvrir(fichier_originale,nom_fichier_originale,'N'))
        {
            printf("mahabch yeteftah");
        }
        if(!ouvrir_TOF(f,fichier_index,'N'))
        {
            printf("impossible d'ouvrir le fichier index");
            return false;
        }
    }
    
    fonctions(nom_fichier_originale) ;
    free(fichier_originale) ;
    
    //afficherTable();
    
    
    return 0;
}

Entete readHeader(){
    FILE * RH = fopen(nom_fichier_originale,"rb");
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
    
    FILE * writer = fopen(nom_fichier_originale,"rb+");
    
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
    
    FILE * f =fopen(nom_fichier_originale,"rb");
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


Entete_TOF readHeaderTOF(){
    FILE * RH = fopen(fichier_index,"rb");
    Entete_TOF H;
    if (RH== NULL) {
        printf("Unable to open the file.\n");
        return H;
    }
   
    fread(&H,sizeof(Entete_TOF),1,RH);
    fclose(RH);

    return H;
}
void setHeader_TOF(int INDEX,int val){
    
    Entete_TOF H = readHeaderTOF();
    
    FILE * writer = fopen(fichier_index,"rb+");
    
    switch(INDEX){
        case 1: 
                H.numDernierBloc=val;
                fwrite(&H,sizeof(Entete_TOF),1,writer);
                fclose(writer);
                return;
                
        case 2: 
        
                H.nbEnreg=val;
                
                fwrite(&H,sizeof(Entete_TOF),1,writer);
                fclose(writer);
                
                return;
        default:
            printf("INDEX NOT VALID [FROM SET_HEADER]");
            return;
    }
    fclose(writer);
}
int getHeader_TOF(int INDEX){
    
    FILE * f =fopen(fichier_index,"rb");
    Entete_TOF H;
    
    if (f== NULL) {
        printf("Unable to open the file.\n");
        fclose(f);
        return -111;        //value of null
    }
    fread(&H, sizeof(H), 1, f);
    switch(INDEX){
        case 1: 
                fclose(f);
                return H.numDernierBloc;
                break;
        case 2: 
                fclose(f);
                return H.nbEnreg;
                break;
        default:
            fclose(f);
            return -222;    //INDEX OverFlow Value
    }
    fclose(f);

}


