#include <stdio.h>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool loadMedia();
void close();

SDL_Window* gWindow = NULL; 
SDL_Surface* gScreenSurface = NULL; 
SDL_Surface* gHelloWorld = NULL; 

//Initialize SDL Video Plugin and Window
bool init() {
    bool success = true;
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        gWindow = SDL_CreateWindow( "SDL_Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL){
            printf("Window could not be created! SDL_Error: %s\n");
            success = false;
        }
        else {
            //Get Surface of the Window
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }
    return success;
}

//Load image into the surface
bool loadMedia() {
    bool success = true;
    gHelloWorld = SDL_LoadBMP("sample.bmp");
    if (gHelloWorld == NULL) {
        printf("Unable to load image! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    return success;
}

//Free up Resources
void close() {
    SDL_FreeSurface( gHelloWorld );
    gHelloWorld = NULL;

    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    SDL_Quit();
}

int main( int argc, char *args[] ) {
    if (!init()) {
        printf("Failed to initialize!\n");
    }
    else {
        if ( !loadMedia() ) {
            printf("Failed to load Media!\n");
        }
        else {
            //Blit transfers copy of src surface to dest surface
            SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
            SDL_UpdateWindowSurface(gWindow); //Render Changes
            SDL_Delay(2000);
        }
    }
    close();
    return 0;
} 