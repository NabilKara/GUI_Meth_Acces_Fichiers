// gcc main.c -o main -lSDL2 -ldl
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include <stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void InitSDL(){
if(SDL_Init(SDL_INIT_VIDEO)<0){
      SDL_Log("erreur initalization %s",SDL_GetError());
     SDL_Quit();
     exit(EXIT_FAILURE);
}
else {
    printf("SDL video system is ready to go \n");
}
}

void InitTFF(){
if(TTF_Init() == -1){
     SDL_Log("erreur initalization SDL2_ttf, error: %s",TTF_GetError());
     SDL_Quit();
     exit(EXIT_FAILURE);
    }else{
        SDL_Log("SDL2_ttf system ready to go!");
    }
}

int main(int argc,char* argv[]){
  SDL_Window* window=NULL;
InitSDL();
int H=700,W=1070;
 window = SDL_CreateWindow("VISUAL TOVC",
                                                                     0,
                                                                     0,
                                                                    W,
                                                                     H,
                                                                     SDL_WINDOW_SHOWN );

    SDL_Renderer* renderer = NULL;
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

 SDL_Surface* screen;

    screen=SDL_GetWindowSurface(window);
    SDL_Surface* image;
    image = SDL_LoadBMP("./textures/purplefluid.bmp");
    SDL_Texture* mymenu=SDL_CreateTextureFromSurface(renderer, image); 
    SDL_FreeSurface(image);
      SDL_Surface* image1;
    image = SDL_LoadBMP("./textures/purplefluid.bmp");
    SDL_Texture* mymeminput=SDL_CreateTextureFromSurface(renderer, image); 
    SDL_FreeSurface(image);
 
InitTFF();
 SDL_Color  black={0,0,0};
SDL_Color red={250,0,0};
SDL_Color green={0,255,0};
//PURPLE{90,14,200};

    TTF_Font* bigFont= TTF_OpenFont("./textures/FrontPageNeue.otf",32);
   
    if(bigFont == NULL){
    printf("could not load the font");
            exit(1);
    }
      SDL_Surface* titleSurf = TTF_RenderText_Solid(bigFont,"VISUAL T/OVC",black);
      SDL_Texture* titleText = SDL_CreateTextureFromSurface(renderer,titleSurf);
    SDL_FreeSurface(titleSurf); 


    TTF_Font* smallFont= TTF_OpenFont("./textures/FrontPageNeue.otf",32);
    
    if(smallFont == NULL){
    printf("could not load the font\n");
            exit(1);
    }
  
  SDL_Surface* playSurf = TTF_RenderText_Solid(smallFont,"play",black);
  SDL_Texture* playTextb = SDL_CreateTextureFromSurface(renderer,playSurf);
SDL_FreeSurface(playSurf); 
 SDL_Surface* playSurfg = TTF_RenderText_Solid(smallFont,"play",green);
  SDL_Texture* playTextg = SDL_CreateTextureFromSurface(renderer,playSurf);
SDL_FreeSurface(playSurfg); 

    SDL_Surface* aboutSurf = TTF_RenderText_Solid(smallFont,"about",black);
  SDL_Texture* aboutText = SDL_CreateTextureFromSurface(renderer,aboutSurf);
SDL_FreeSurface(aboutSurf); 

SDL_Surface* exitSurf = TTF_RenderText_Solid(smallFont,"exit",black);
  SDL_Texture* exitText = SDL_CreateTextureFromSurface(renderer,exitSurf);
SDL_FreeSurface(exitSurf);
SDL_Surface* exitSurfr = TTF_RenderText_Solid(smallFont,"exit",red);
  SDL_Texture* exitTextr = SDL_CreateTextureFromSurface(renderer,exitSurf);
SDL_FreeSurface(exitSurfr);


int carWidth = 33;

SDL_Rect titleRec;
    titleRec.x = 380;
    titleRec.y = 130;
    titleRec.w = 310;
    titleRec.h = 120;

SDL_Rect playRec;
    playRec.x = 460;
    playRec.y =360 ;
    playRec.w = carWidth*4;
    playRec.h = 50;

SDL_Rect aboutRec;
    aboutRec.x = 450;
    aboutRec.y = 440;
    aboutRec.w = carWidth*5;
    aboutRec.h = 50;

SDL_Rect exitRec;
    exitRec.x = 460;
    exitRec.y = 520;
    exitRec.w = carWidth*4;
    exitRec.h = 50;


bool inMenu =true;
bool inPlay =false;

int mouseX,mouseY;
    Uint32 buttons;
while (inMenu) {
    buttons =SDL_GetMouseState(&mouseX, &mouseY);

          SDL_RenderClear(renderer);
       SDL_RenderCopy(renderer,mymenu,NULL,NULL);


    SDL_Event event;
      
        while (SDL_PollEvent(&event)){
                switch (event.type) {
                    case  SDL_QUIT :
                      inMenu=false;
          
                 case SDL_MOUSEMOTION:  
                    mouseX=event.motion.x;
                    mouseY=event.motion.y;
                }
                
               switch (event.button.button) {
            
                case  SDL_BUTTON_LEFT :
                 case SDL_BUTTON_RIGHT :
                    if (  mouseX >= playRec.x && mouseX <= playRec.x + playRec.w && mouseY >= playRec.y && mouseY <= playRec.y + playRec.h ){
                    inMenu=false;
                    inPlay =true;
                    printf("play\n");
                    SDL_Delay(300);
                    }
                    else if (mouseX >= exitRec.x && mouseX <= exitRec.x + exitRec.w && mouseY >= exitRec.y && mouseY <= exitRec.y + exitRec.h  ){
                    printf("exit\n");
                     inMenu=false;

                   }
               }              
}
        SDL_RenderCopy(renderer,titleText,NULL,&titleRec);
  if (mouseX >= playRec.x && mouseX <= playRec.x + playRec.w && mouseY >= playRec.y && mouseY <= playRec.y + playRec.h){
                    SDL_RenderCopy(renderer,playTextg,NULL,&playRec);
          
                }else {
                       SDL_RenderCopy(renderer,playTextb,NULL,&playRec);

                      }
        
                        if (mouseX >= exitRec.x && mouseX <= exitRec.x + exitRec.w && mouseY >= exitRec.y && mouseY <= exitRec.y + exitRec.h){
                                    SDL_RenderCopy(renderer,exitTextr,NULL,&exitRec);
                                    
                    }
                        else{
                    SDL_RenderCopy(renderer,exitText,NULL,&exitRec);
                         }                    
        SDL_RenderCopy(renderer, aboutText, NULL, &aboutRec);
        SDL_RenderPresent(renderer);
}
    SDL_DestroyTexture(mymenu);
    SDL_DestroyTexture(mymeminput);
    SDL_DestroyTexture(titleText);
    SDL_DestroyTexture(playTextb);
    SDL_DestroyTexture(exitText);
    SDL_DestroyTexture(aboutText);
if(inPlay){
    int blocSize;
    int memorySize;

 SDL_Surface* image;
 
    image = SDL_LoadBMP("./textures/interyourblocsize.bmp");
    SDL_BlitSurface(image,NULL,screen,NULL);
    SDL_FreeSurface(image);
    SDL_UpdateWindowSurface(window);

     scanf("%d",&blocSize);
    SDL_Delay(300);
    

    SDL_Surface* image1;
    image1 = SDL_LoadBMP("./textures/internumberofblocs.bmp");
    SDL_BlitSurface(image1,NULL,screen,NULL);
    SDL_FreeSurface(image1);
    SDL_UpdateWindowSurface(window);
    scanf("%d",&memorySize);

SDL_DestroyWindow(window);
    window = SDL_CreateWindow("VISUAL TOVC",
                                                                     0,
                                                                     0,
                                                                    W,
                                                                     H,
                                                                     SDL_WINDOW_SHOWN );
        SDL_Delay(300);
renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
 
   image1 = SDL_LoadBMP("./textures/purplefluidmainwithbufferexemple.bmp");
    SDL_Texture*  mainText =SDL_CreateTextureFromSurface(renderer, image1);
    SDL_FreeSurface(image1);

    SDL_Rect createfileRec;
    createfileRec.x=0;
    createfileRec.y=0;
    createfileRec.h=0;
    createfileRec.w=0;

    SDL_Rect deletefileRec;
    deletefileRec.x=0;
    deletefileRec.y=0;
    deletefileRec.h=0;
    deletefileRec.w=0;

        SDL_Rect addengRec;
    addengRec.x=0;
    addengRec.y=0;
    addengRec.h=0;
    addengRec.w=0;

    SDL_Rect deleteengRec;
    deleteengRec.x=0;
    deleteengRec.y=0;
    deleteengRec.h=0;
    deleteengRec.w=0;

        SDL_Rect modengRec;
    modengRec.x=0;
    modengRec.y=0;
    modengRec.h=0;
    modengRec.w=0;

    SDL_Rect searchindexRec;
    searchindexRec.x=0;
    searchindexRec.y=0;
    searchindexRec.h=0;
    searchindexRec.w=0;

    SDL_Rect engRec;
    engRec.x=0;
    engRec.y=0;
    engRec.h=0;
    engRec.w=0;



//coordinate of first physiacal (enregistrement hhhh):(150,264)(500+50,314+40)
//coordinate of premiere entete physique:(100,264)(200,400)
SDL_Surface *entetelibreSurf = SDL_LoadBMP("./textures/emptyentete.bmp");
SDL_SetColorKey(entetelibreSurf, SDL_TRUE,  SDL_MapRGB(entetelibreSurf->format,255,255,250));
SDL_Texture* entetelibreText =SDL_CreateTextureFromSurface(renderer, entetelibreSurf);
SDL_FreeSurface(entetelibreSurf);

SDL_Surface *fullenteteSurf = SDL_LoadBMP("./textures/fullentete.bmp");
SDL_SetColorKey(fullenteteSurf, SDL_TRUE,  SDL_MapRGB(fullenteteSurf->format,255,255,250));
SDL_Texture* fullenteteText =SDL_CreateTextureFromSurface(renderer, fullenteteSurf);
SDL_FreeSurface(fullenteteSurf);

SDL_Surface *entetchevaucherSurf = SDL_LoadBMP("./textures/chevauchedentete.bmp");
SDL_SetColorKey(entetchevaucherSurf, SDL_TRUE,  SDL_MapRGB(entetchevaucherSurf->format,255,255,250));
SDL_Texture* entetchevaucherText =SDL_CreateTextureFromSurface(renderer, entetchevaucherSurf);
SDL_FreeSurface(entetchevaucherSurf);
//coordinate of buffer example : (600,500)(1000,600)

//creating our tilemap (memory)
SDL_Surface *memorySurface = SDL_LoadBMP("./textures/buffer.bmp");
SDL_SetColorKey(memorySurface, SDL_TRUE,  SDL_MapRGB(memorySurface->format,255,255,250));
SDL_Texture* memoryTexture =SDL_CreateTextureFromSurface(renderer, memorySurface);
SDL_FreeSurface(memorySurface);
//int entettab[memorySize];

SDL_Rect entetephysique[memorySize];
    for (int i=0;i<memorySize;i++){
                entetephysique[i].x=50;
                entetephysique[i].y=(i)*100+264  ;
                entetephysique[i].w=100;
                entetephysique[i].h=100;

}



SDL_Rect memory[memorySize];
for (int i=0;i<memorySize;i++){
        memory[i].x=150;
                memory[i].y=(i)*100+264  ;
                memory[i].w=400;
                memory[i].h=100;

}



int offset=0;
 const Uint8* state = SDL_GetKeyboardState(NULL);
while (inPlay) {

                       SDL_RenderCopy(renderer,mainText,NULL,NULL);
                       
SDL_Event event;
      
        while (SDL_PollEvent(&event)){          
                switch (event.type) {
                    case  SDL_QUIT :
                      inPlay=false;
                 case SDL_MOUSEMOTION:  
                    mouseX=event.motion.x;
                    mouseY=event.motion.y;             
 
                    
                  
                }
        }
                  if (state[SDL_SCANCODE_DOWN]){
                        offset++;
                        printf("key down");
                        SDL_Delay(200);
                    }
                     if (state[SDL_SCANCODE_UP]){
                         printf("key down");
                        offset--;
                     SDL_Delay(200);
                        }
                           if(offset<0){
                            offset=0;
                           }
                           if (offset>memorySize*100){
                            offset=memorySize*100;
                           }
                

for (int i=0;i<memorySize;i++){
          memory[i].y=(i-offset)*100+264  ;
             entetephysique[i].y=(i-offset)*100+264  ;
}
for (int i=0;i<memorySize;i++){
        SDL_RenderCopy(renderer, memoryTexture, NULL, &memory[i]);
        //pour afficher lentete physique en la remplace apres par un swithc case qui lit du tableau entetetab prent trois valeur chevaucher plein et vide
                SDL_RenderCopy(renderer, entetelibreText, NULL, &entetephysique[i]);

    }

SDL_RenderPresent(renderer);


}
SDL_DestroyTexture(mainText);
SDL_DestroyTexture(memoryTexture);
}


SDL_DestroyWindow(window);
    TTF_CloseFont(smallFont);

    TTF_CloseFont(bigFont);
SDL_Quit();

return 0;
}
