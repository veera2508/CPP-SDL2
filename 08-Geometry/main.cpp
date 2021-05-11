#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
            else {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize loading for png
                int imgFlags = IMG_INIT_PNG; //flag for png loading
                //We pass the flags required to load and it returns flags successfully loaded
                if ( !( IMG_Init( imgFlags ) & imgFlags ) ) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
        
    }
    return success;
}

//Load image and return texture
SDL_Texture* loadTexture( std::string path ) {
    //Surface to store the optimized image
    SDL_Texture* newTexture = NULL;

    //Surface to store the image
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if ( loadedSurface == NULL )
        printf( "Unable to load image! SDL Error: %s\n", IMG_GetError() );
    
    else {
        //Create texture from surface pixels

        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) 
            printf("Unable to create texture! SDL Error:%s\n", SDL_GetError());
        
        //get rid of old surface
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

bool loadMedia() {
    bool success = true;

    gTexture = loadTexture( "texture.png" );
	if( gTexture == NULL ){
		printf( "Failed to load texture image!\n" );
		success = false;
	}

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
                //Clear Screen
                SDL_RenderClear( gRenderer );
                //Copy portion or whole of texture to the renderer
                SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
                //Update the renderer
                SDL_RenderPresent( gRenderer );
            }
        }
    }
    close();
    return 0;
} 