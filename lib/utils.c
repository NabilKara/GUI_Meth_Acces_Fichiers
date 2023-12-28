#include<stdio.h>
#include<stdlib.h>
#include<utils.h>

int strToInt(char ch[],int taille)
{
    int Int = 0 ;
    
    for (int i = 0; i < taille; i++)
    {
        Int = Int * 10 + (ch[i] - 48) ;
    }
    
    return Int;
    
}

int nbPos(int n)
{
    if (n > 999)
    {
        return -1 ; // faux
    }
    
    int i=0 ;
    while (n != 0)
    {
        i = i + 1 ;
        n = n / 10 ;
    }

    return i;

}

void intToStr(int n,char ch[])
{ 
    if(nbPos(n) == -1)
    {
        printf("ERREUR !!!!");
        exit(1) ;
    }
    else
    {
        
        for (int i = 2; i >= 0; i--) // 2 car le nb max est : 999 
        {
            ch[i] = n % 10 + 48 ; 
            n = n / 10 ;
        }
        
    }
}