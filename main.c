#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <asm-generic/errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>





int main(int argc,char * argv[])
{
      //vars
      int Mx = 0;
      int My = 0;
      bool play = false;
      bool about = false;
      bool shouldexit = false;

//CREATING THE WINDOW      
  SDL_Window* window =NULL;
             if(SDL_Init(SDL_INIT_VIDEO)<0){
                         printf("SDL could not be initialized:  ");
                         SDL_GetError();
             }
            else {
                        printf("SDL video system is ready to go \n");
             }
window = SDL_CreateWindow("mainwindow",0, 0, 640,480, SDL_WINDOW_SHOWN);
SDL_Renderer* renderer = NULL;
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);      
      //init menu
 //init background
      SDL_Texture *background = IMG_LoadTexture(renderer, "FilePath");
      SDL_Rect background_rect;
          background_rect x = 0;   
          background_rect y = 0;   
          background_rect w = 500;
          background_rect h = 500;

//The New Game button
          SDL_Texture *newGame = IMG_LoadTexture(renderer, "FilePath");

                SDL_Rect newGame_Rect;
                 newGame_Rect.x = 0;
                 newGame_Rect.y = 100;
                 newGame_Rect.w = 50;
                 newGame_Rect.h = 25;

//exit button
                SDL_Texture *exit = IMG_LoadTexture(renderer, "FilePath");

                SDL_Rect exit_Rect;
                exit_Rect.x = 0;
                exit_Rect.y = 200;
                exit_Rect.w = 50;
                exit_Rect.h = 25;

//about button
                SDL_Texture *about = IMG_LoadTexture(renderer, "FilePath");

                SDL_Rect about_Rect;
                about_Rect.x = 0;
                about_Rect.y = 300;
                about_Rect.w = 50;
                about_Rect.h = 25;



 
      while (! shouldexit){
      
           SDL_Event event;
        // start event loop
        while (SDL_PollEvent(&event)){

            
       // new game has been clicked????
               SDL_GetMouseState(&Mx, &My); 
       // check if cursor on new game
               if (Mx >= newGame_Rect.x && Mx <= newGame_Rect.x + newGame_Rect.w && My >= newGame_Rect.y && My <= newGame_Rect.y + newGame_Rect.h) 
                           {
                            // color new game
                 SDL_SetTextureColorMod(newGame, 250, 0, 0 ); 
    
//check if clicked
if (event->type == SDL_MOUSEBUTTONDOWN)   
{
    if (event->button.button == SDL_BUTTON_LEFT)
    {    
        play = true;
    }
}
}
else 
{
 //make color white if note pressed
 SDL_SetTextureColorMod(Gettexture(), 250, 250, 250);
}


       // exit has been clicked????
if (Mx >= exit_Rect.x && Mx <= exit_Rect.x + exit_Rect.w && My >= exit_Rect.y && My <= exit_Rect.y + exit_Rect.h) 
                           {
                            // color new game
                 SDL_SetTextureColorMod(exit, 250, 0, 0 ); 
    
//check if clicked
if (event->type == SDL_MOUSEBUTTONDOWN)   
{
    if (event->button.button == SDL_BUTTON_LEFT)
    {    
      //quiting
          SDL_DestroyWindow(window);
            SDL_Quit();
        exit(0);
    }
}
}
else 
{
 //make color white if note pressed
 SDL_SetTextureColorMod(Gettexture(), 250, 250, 250); 
    
}
       // about has been clicked????
if (Mx >= about_Rect.x && Mx <= about_Rect.x + about_Rect.w && My >= about_Rect.y && My <= about_Rect.y + about_Rect.h) 
                           {
                            // color new game
                 SDL_SetTextureColorMod(exit, 250, 0, 0 ); 
    
//check if clicked
if (event->type == SDL_MOUSEBUTTONDOWN)   
{
    if (event->button.button == SDL_BUTTON_LEFT)
    {    
        about = true;
    }
}
}
else 
{
 //make color white if note pressed
 SDL_SetTextureColorMod(Gettexture(), 250, 250, 250); 
    
}


//click close button
   if (event.type ==SDL_QUIT){
                    shouldexit=true;
   }
        }
      }
  }

