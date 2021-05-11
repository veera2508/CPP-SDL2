#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>

enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool loadMedia();
void close();
SDL_Surface* loadSurface( std::string path );


SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];
SDL_Surface* gCurrentSurface = NULL;

//Load image and return surface
SDL_Surface* loadSurface( std::string path ) {
    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    if ( loadedSurface == NULL )
        printf( "Unable to load image! SDL Error: %s\n", SDL_GetError() );
    return loadedSurface;
}

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
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }
    return success;
}

bool loadMedia() {
    bool success = true;

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface("images/press.bmp");
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL ) {
        printf( "Failed to load default image!\n" );
        success = false;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = loadSurface("images/up.bmp");
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL ) {
        printf( "Failed to load up image!\n" );
        success = false;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = loadSurface("images/down.bmp");
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL ) {
        printf( "Failed to load down image!\n" );
        success = false;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = loadSurface("images/left.bmp");
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL ) {
        printf( "Failed to load left image!\n" );
        success = false;
    }

    gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = loadSurface("images/right.bmp");
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL ) {
        printf( "Failed to load right image!\n" );
        success = false;
    }

    return success;
}


//Free up Resources
void close() {
    SDL_FreeSurface( gCurrentSurface );
    gCurrentSurface = NULL;

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
            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
            //Main Loop
            while (!quit) {
                //Loop to get events from event queue
                while (SDL_PollEvent( &e ) != 0) {
                    //SDL_Quit event is pressing X
                    if (e.type == SDL_QUIT)
                        quit = true;
                    //SDL key press event
                    else if (e.type == SDL_KEYDOWN) {
                        //Switch over all the keys
                        switch(e.key.keysym.sym) {
                            case SDLK_UP:
                            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
                            break;
                            case SDLK_DOWN:
                            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
                            break;
                            case SDLK_LEFT:
                            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
                            break;
                            case SDLK_RIGHT:
                            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
                            break;
                            case SDLK_ESCAPE:
                            quit = true;
                            break;
                            default:
                            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
                            break;
                        }
                    }
                }
                //Blit transfers copy of src surface to dest surface
                SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }
    close();
    return 0;
} 