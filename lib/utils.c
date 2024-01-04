#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<utils.h>

int strToInt(char ch[])
{
    return atoi(ch);
}

void intToStr(int n,char ch[])
{ 
    if(n > 999)
    {
        printf("ERREUR !!!!");
        exit(1) ;
    }
    else
    {
        sprintf(ch , "%d" , n) ;
    }
}