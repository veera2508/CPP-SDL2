#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool loadMedia();
void close();
SDL_Surface* loadSurface( std::string path );


SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gPNGSurface = NULL;

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
            //Initialize loading for png
            int imgFlags = IMG_INIT_PNG; //flag for png loading
            //We pass the flags required to load and it returns flags successfully loaded
            if ( !( IMG_Init( imgFlags ) & imgFlags ) ) {
                printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                success = false;
            }
            else {
                //Get window surface
                gScreenSurface = SDL_GetWindowSurface( gWindow );
            }
        }
        
    }
    return success;
}

//Load image and return surface (Optimized)
SDL_Surface* loadSurface( std::string path ) {
    //Surface to store the optimized image
    SDL_Surface* optimizedSurface = NULL;

    //Surface to store the image
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if ( loadedSurface == NULL )
        printf( "Unable to load image! SDL Error: %s\n", IMG_GetError() );
    
    else {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
        if (optimizedSurface == NULL) 
            printf("Cannot optimize image! SDL Error:%s\n", SDL_GetError());
        SDL_FreeSurface(loadedSurface);
    }
    return optimizedSurface;
}

bool loadMedia() {
    bool success = true;

    gPNGSurface = loadSurface( "loaded.png" );
	if( gPNGSurface == NULL ){
		printf( "Failed to load PNG image!\n" );
		success = false;
	}

	return success;
}


//Free up Resources
void close() {
    SDL_FreeSurface( gPNGSurface);
    gPNGSurface = NULL;

    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    IMG_Quit();
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
                //Blit transfers copy of src surface to dest surface
                SDL_BlitSurface(gPNGSurface, NULL, gScreenSurface, NULL);
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }
    close();
    return 0;
} 