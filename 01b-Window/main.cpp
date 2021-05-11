#include <stdio.h>
#include <SDL2/SDL.h>

//Window Dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main( int argc, char *args[] ) {
    SDL_Window* window = NULL;  //Window object
    SDL_Surface* screenSurface = NULL;  //Surface object

    //Check if SDL Video Plugin can be Initialized
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {

        //Create a new Window and check if it is Initialized
        window = SDL_CreateWindow( "SDL_Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL){
            printf("Window could not be created! SDL_Error: %s\n");
        }
        else {
            //Get Surface of the Window
            screenSurface = SDL_GetWindowSurface( window );
            SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00 ) ); //Fill the surface with a rectangle with color RGB
            SDL_UpdateWindowSurface( window ); //Render Changes
            SDL_Delay( 2000 );
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
} 