#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool loadMedia();
void close();
SDL_Texture* loadTexture( std::string path );


SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

//Initialize SDL Video Plugin and Window
bool init() {
    bool success = true;
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        gWindow = SDL_CreateWindow( "SDL_Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n");
            success = false;
        }
        else {
            //Create renderer for window instead of surface
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if ( gRenderer == NULL ) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
        }  
    }
    return success;
}


bool loadMedia() {
    bool success = true;
	return success;
}


//Free up Resources
void close() {
    SDL_DestroyTexture( gTexture );
    gTexture = NULL;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

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
            bool quit = false;
            SDL_Event e; //Variable to Store Event
            //Main Loop
            while (!quit) {
                //Loop to get events from event queue
                while (SDL_PollEvent( &e ) != 0) {
                    //SDL_Quit event is pressing X
                    if (e.type == SDL_QUIT)
                        quit = true;
                }
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                //Clear Screen
                SDL_RenderClear( gRenderer );
                //Create and Draw rectangle filled with red
                SDL_Rect fillRect = {SCREEN_WIDTH/4, SCREEN_HEIGHT/4, SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                SDL_RenderFillRect( gRenderer, &fillRect );
                //Create and Draw hollow rectangle with black outline
                SDL_Rect outlineRect = {SCREEN_WIDTH/6, SCREEN_HEIGHT/6, 2*SCREEN_WIDTH/3, 2*SCREEN_HEIGHT/3};
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
                SDL_RenderDrawRect( gRenderer, &outlineRect );
                //Draw green horizontal line
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0x00 );
                SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2 );
                //Draw vertical line of blue dots
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0x00 );
                for (int i = 0; i < SCREEN_HEIGHT; i+=5) {
                    SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH/2, i );
                }
                //Update the renderer
                SDL_RenderPresent( gRenderer );
            }
        }
    }
    close();
    return 0;
} 