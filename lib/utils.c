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
void intToStr(int n,char ch[]);