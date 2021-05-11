#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum PORT_NO {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM,
    PORT_TOTAL
};

bool init();
bool loadMedia();
void close();
void initViewPorts();
SDL_Texture* loadTexture( std::string path );


SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;
SDL_Rect* gViewPorts[PORT_TOTAL];

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

    gTexture = loadTexture( "viewport.png" );
	if( gTexture == NULL ){
		printf( "Failed to load texture image!\n" );
		success = false;
	}

	return success;
}

void initViewPorts() {
    //Top Left Viewport
    gViewPorts[TOP_LEFT] = (SDL_Rect*) malloc(sizeof(SDL_Rect));
    gViewPorts[TOP_LEFT]->x = 0;
    gViewPorts[TOP_LEFT]->y = 0;
    gViewPorts[TOP_LEFT]->w = SCREEN_WIDTH/2;
    gViewPorts[TOP_LEFT]->h = SCREEN_HEIGHT/2;

    //Top Right Viewport
    gViewPorts[TOP_RIGHT] = (SDL_Rect*) malloc(sizeof(SDL_Rect));
    gViewPorts[TOP_RIGHT]->x = SCREEN_WIDTH/2;
    gViewPorts[TOP_RIGHT]->y = 0;
    gViewPorts[TOP_RIGHT]->w = SCREEN_WIDTH/2;
    gViewPorts[TOP_RIGHT]->h = SCREEN_HEIGHT/2;

    //Bottom Viewport
    gViewPorts[BOTTOM] = (SDL_Rect*) malloc(sizeof(SDL_Rect));
    gViewPorts[BOTTOM]->x = 0;
    gViewPorts[BOTTOM]->y = SCREEN_HEIGHT/2;
    gViewPorts[BOTTOM]->w = SCREEN_WIDTH;
    gViewPorts[BOTTOM]->h = SCREEN_HEIGHT/2;
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
            initViewPorts();
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
                //Top left viewport
                SDL_RenderSetViewport( gRenderer, gViewPorts[TOP_LEFT] );
                SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
                //Top right viewport
                SDL_RenderSetViewport( gRenderer, gViewPorts[TOP_RIGHT] );
                SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
                //Bottom viewport
                SDL_RenderSetViewport( gRenderer, gViewPorts[BOTTOM] );
                SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
                //Update the renderer
                SDL_RenderPresent( gRenderer );
            }
        }
    }
    close();
    return 0;
} 