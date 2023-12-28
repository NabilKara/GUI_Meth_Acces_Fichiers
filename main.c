#include <SDL2/SDL_error.h>
#include <stdlib.h>
#include<stdio.h>

#include <SDL2/SDL.h>
int main()
{

  if (SDL_Init(SDL_INIT_VIDEO)<0){  
        printf("SDL could not be initialized");
        SDL_GetError();
  }else {
        printf("SDL READY \n");

  }


  }

