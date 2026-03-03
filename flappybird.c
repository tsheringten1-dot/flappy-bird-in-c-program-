#include<stdio.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h> // include ttf for on screen text
#include<stdlib.h>

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 600;
int score=0;

typedef struct{
   float y;
   float velocity;
   
}Bird;

typedef struct
{
   float x;
   float height;
   int pass;
}Pipe;



Bird bird={300.0f,0.0f};
const float gravity=0.25f;
const float jump_strength = -6.0f;
const float pipe_gap=150;
const float pipe_width=60;
const float pipe_speed = 5.0f; //Speed of the pipes
Pipe pipes[3];

TTF_Font *font = NULL;  //Added for text
//Simple Helper function to draw text on screen
void drawText(SDL_Renderer *renderer, const char *text, int x, int y,SDL_Color color,int size)
{
   if(!font) return;
   TTF_Font *tempFont = TTF_OpenFont("font.ttf",size);
   if(!tempFont) return;
   SDL_Surface *surface= TTF_RenderText_Solid(tempFont,text,color);
   if(!surface){TTF_CloseFont(tempFont); return;}
   SDL_Texture *texture =SDL_CreateTextureFromSurface(renderer,surface);
   SDL_Rect rect={x-surface->w/2,y,surface->w,surface->h}; // surface->w, surface->h
   SDL_RenderCopy(renderer,texture,NULL,&rect);

   SDL_FreeSurface(surface);
   SDL_DestroyTexture(texture);
   TTF_CloseFont(tempFont);
}

int main(int argc, char* argv[])
 //1.Initialize SDL(Video subsystem)
{
   if(SDL_Init(SDL_INIT_VIDEO)<0)
   {
   printf("SDL could not initialize! Error:%s\n",SDL_GetError());
   }

   //Initilizing TTF
   if(TTF_Init()<0)
   {printf("TTF could not initialize! Eror:%s\n",TTF_GetError());
   }
   font =TTF_OpenFont("font.ttf",48);
   if(!font){
      printf("font.ttf not found\n");
      printf("Dowenload any .ttf file rename it to font.ttf\n");
   }
   //2.Create the window
   SDL_Window* window = SDL_CreateWindow("My Game window",
    SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);

    if(window == NULL)
   {
    printf("Window could not be created! Error:%s\n",SDL_GetError());
   }
   //3.Creating the renderer
   SDL_Renderer* renderer= SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
   
   if(renderer==NULL)
   {
      printf("Renderer could not be created! Error:%s\n",SDL_GetError());
   }
   
   //----- Initialize pipes-----
   for (int i = 0; i < 3; i++)
   {
     pipes[i].x=SCREEN_WIDTH+(i*250);
     pipes[i].height=(rand()%250)+100;// Random Height between 100-350
   }
   
      
   //The Game Loop
   int running =1; //A flag to keep the loop going
   SDL_Event e;  //container to store all the events 
   
   
   while (running)
   {
      //Handle Inputs
      while (SDL_PollEvent(&e))
      {
         //Did the user click the 'X' button
        if(e.type == SDL_QUIT){
         running=0; //Stop the loop
        }
        if ((e.type==SDL_MOUSEBUTTONDOWN||e.type==SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE||e.type==SDL_FINGERDOWN))
        {
             bird.velocity=jump_strength;
        }

      }
      //----Physics----
      bird.velocity=bird.velocity+gravity;
      bird.y=bird.y+bird.velocity;

      for (int i = 0; i < 3; i++)
      {
        
         //Moves pipe left
         pipes[i].x=pipes[i].x-pipe_speed;
          if(pipes[i].x + pipe_width<50 && pipes[i].pass==0)
          {
            score++;
            pipes[i].pass=1;
            printf("Score: %d\n",score);
          }
         //Recycle pipe if it leaves the screen
         if(pipes[i].x<-pipe_width){
            pipes[i].x=pipes[i].x+(3*250);
            pipes[i].height=(rand()%250)+100;
            pipes[i].pass=0;
         }
      }
      
     
      if(bird.y<0||bird.y>(SCREEN_HEIGHT-30))
      {  
        SDL_Color red = {255,0,0,255};
         drawText(renderer,"GAME OVER",SCREEN_WIDTH/2,200,red,50);
         SDL_RenderPresent(renderer);
        printf("GAME OVER! RESETING GAME....\n");
        SDL_Delay(800);
        score=0;
         bird.y=300.0f;
         bird.velocity=0.0f;
         
      }   
      
     //Change the color of screen to sky blue
     SDL_SetRenderDrawColor(renderer,135,206,235,255);
     SDL_RenderClear(renderer);

     // sun
     SDL_SetRenderDrawColor(renderer,255,255,0,255);

     SDL_Rect s1= {315,70,30,15};
     SDL_RenderFillRect(renderer, &s1);
      SDL_Rect s2= {300,85,60,25};
     SDL_RenderFillRect(renderer, &s2);
      SDL_Rect s3= {315,110,30,15};
     SDL_RenderFillRect(renderer, &s3);
     // -------- DRAW CLOUDS --------
SDL_SetRenderDrawColor(renderer,255,255,255,255);

// Cloud 1
SDL_Rect c1 = {80, 80, 60, 30};
SDL_Rect c2 = {60, 90, 50, 25};
SDL_Rect c3 = {120, 90, 50, 25};
SDL_RenderFillRect(renderer, &c1);
SDL_RenderFillRect(renderer, &c2);
SDL_RenderFillRect(renderer, &c3);

// Cloud 2
SDL_Rect c4 = {250, 150, 70, 35};
SDL_Rect c5 = {230, 165, 50, 25};
SDL_Rect c6 = {300, 165, 50, 25};
SDL_RenderFillRect(renderer, &c4);
SDL_RenderFillRect(renderer, &c5);
SDL_RenderFillRect(renderer, &c6);

      //define the bird
     SDL_Rect bird_rect={50,(int)bird.y,30,30};// x=50,y=300
     
     //drawing the pipe
     SDL_SetRenderDrawColor(renderer,34,139,34,255);
     
     for (int i = 0; i < 3; i++)
     {
      //top pipe
      SDL_Rect top_pipe={(int)pipes[i].x,0,(int)pipe_width,(int)pipes[i].height};
      // Top pipe head (cap)
      SDL_Rect top_cap = {(int)pipes[i].x - 10 ,(int)pipes[i].height ,(int)pipe_width + 20,20};
      SDL_RenderFillRect(renderer, &top_cap);

      //bottom pipe
      SDL_Rect bottom_pipe={(int)pipes[i].x,(int)pipes[i].height+pipe_gap,(int)pipe_width,(SCREEN_HEIGHT-((int)pipes[i].height+pipe_gap))};
      SDL_RenderFillRect(renderer,&top_pipe);
      SDL_RenderFillRect(renderer,&bottom_pipe);
      // bottom pipe head cap
      SDL_Rect bottom_cap = {(int)pipes[i].x - 10,(int)pipes[i].height + pipe_gap,(int)pipe_width + 20,20};
      SDL_RenderFillRect(renderer, &bottom_cap);
      // collision logic
      if(SDL_HasIntersection(&bird_rect,&top_pipe) || SDL_HasIntersection(&bird_rect,&bottom_pipe))
      {
         SDL_Color red = {255,0,0,255};
         drawText(renderer,"GAME OVER",SCREEN_WIDTH/2,200,red,50);
         SDL_RenderPresent(renderer);
        printf("GAME OVER! RESETING GAME....\n");
        SDL_Delay(800);
         score=0;
         bird.y=300.0f;
         bird.velocity=0.0f;
           for (int i = 0; i < 3; i++)
             {
            pipes[i].x=SCREEN_WIDTH+(i*250);
            pipes[i].height=(rand()%250)+100;
            pipes[i].pass=0;
            }
        
      }

     }
     
    
     //Draw the bird(Yellow)
     SDL_SetRenderDrawColor(renderer,255,255,0,255);
     SDL_RenderFillRect(renderer,&bird_rect);
      //Draw the wings
      SDL_SetRenderDrawColor(renderer, 255,255,255,255);
      SDL_Rect wing = {60, (int)bird.y + (int)(bird.velocity * 2) + 10, 15, 25};

      SDL_RenderFillRect(renderer,&wing);
   
      // eyes
      SDL_SetRenderDrawColor(renderer,0,0,0,255);
      SDL_Rect eyes={65,(int)bird.y+5, 5,5};
      SDL_RenderFillRect(renderer,&eyes);
 //Draw scoreboard
     char scoreText[16];
     sprintf(scoreText,"%d",score);
     SDL_Color white ={255,255,255,255};
     drawText(renderer,scoreText,SCREEN_WIDTH/2,30,white,60);

     //show the result
     SDL_RenderPresent(renderer);
     SDL_Delay(16);//60 fps
   }
   //Cleanup
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   SDL_Quit(); 

}