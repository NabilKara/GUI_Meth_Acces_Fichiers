#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<utils.h>
#include<TŌVC.h>

int strToInt(char ch[])
{
    return atoi(ch);
}

void intToStr(int n,char ch[])
{ 
    if(n < 0 || n > MAX_NO_CHARS)
    {
        printf("ERREUR !!!!");
        exit(1) ;
    }
    else
    {
        sprintf(ch , "%d" , n) ;
    }
}