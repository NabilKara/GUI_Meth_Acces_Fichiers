#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#include"TnOVC.h"
#include"functions.h"

#include "utils.h"


int main()
{
    TnOVC *f = malloc(sizeof(TnOVC)) ;
    Buffer *buf = malloc(sizeof(Buffer));

    

    char ch[] = "test1 test2 test3" ;
    if(!ouvrir(f,"Blocs.bin",'N')){
        printf("erreur");
    }
    else{
        printf("no erreur");
    }
    
    int t=3;
    while(t--)
        inserer(ch,strlen(ch),"Blocs.bin",404);
        
    return 0;
}
