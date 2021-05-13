#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

enum Clips {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    CLIPS_TOTAL
};

//Wrapper class for texture
class LTexture {
    public:
        //Constructor
        LTexture();

        //Destructor
        ~LTexture();

        //Load image into texture
        bool loadFromFile( std::string path );

        //Dealocates texture
        void free();

        //Renders texture at a given point
        void render( int x, int y, SDL_Rect* clip );

        //Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        //Hardware Texture
        SDL_Texture* mTexture;

        //Image Dimensions
        int mWidth;
        int mHeight;
};

LTexture::LTexture() {
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture() {
    free();
}

bool LTexture::loadFromFile( std::string path ) {
    //Delete the previous texture
    free();
    SDL_Texture* newTexture = NULL;
    //Surface to store the image
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if ( loadedSurface == NULL )
        printf( "Unable to load image! SDL Error: %s\n", IMG_GetError() );
    
    else {
        //Create texture from surface pixels

        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ));
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) 
            printf("Unable to create texture! SDL Error:%s\n", SDL_GetError());

        else {
            //Store image dimesions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        
        //get rid of old surface
        SDL_FreeSurface(loadedSurface);
    }
    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::free() {
    //Free texture if it exists
    if (mTexture != NULL) {
        SDL_DestroyTexture( mTexture );
        mHeight = 0;
        mWidth = 0;
    }
}

void LTexture::render( int x, int y, SDL_Rect* clip ) {
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    //Set clip rendering dimensions
    if ( clip != NULL ) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}

int LTexture::getHeight() {
    return mHeight;
}

int LTexture::getWidth() {
    return mWidth;
}

SDL_Rect* gSpriteClips[CLIPS_TOTAL];
LTexture gSpriteSheetTexture;

bool loadMedia() {
    bool success = true;

    //Load Foo texture
    if ( !gSpriteSheetTexture.loadFromFile("dots.png") ) {
        printf("Failed to load sprite sheet!\n");
        success = false;
    }
    return success;
}

void initSpriteClips() {
    //Top Left Viewport
    gSpriteClips[TOP_LEFT] = (SDL_Rect*) malloc(sizeof(SDL_Rect));
    gSpriteClips[TOP_LEFT]->x = 0;
    gSpriteClips[TOP_LEFT]->y = 0;
    gSpriteClips[TOP_LEFT]->w = 100;
    gSpriteClips[TOP_LEFT]->h = 100;

    //Top Right Viewport
    gSpriteClips[TOP_RIGHT] = (SDL_Rect*) malloc(sizeof(SDL_Rect));
    gSpriteClips[TOP_RIGHT]->x = 100;
    gSpriteClips[TOP_RIGHT]->y = 0;
    gSpriteClips[TOP_RIGHT]->w = 100;
    gSpriteClips[TOP_RIGHT]->h = 100;

    //Bottom Left Viewport
    gSpriteClips[BOTTOM_LEFT] = (SDL_Rect*) malloc(sizeof(SDL_Rect));
    gSpriteClips[BOTTOM_LEFT]->x = 0;
    gSpriteClips[BOTTOM_LEFT]->y = 100;
    gSpriteClips[BOTTOM_LEFT]->w = 100;
    gSpriteClips[BOTTOM_LEFT]->h = 100;

    //Bottom Viewport
    gSpriteClips[BOTTOM_RIGHT] = (SDL_Rect*) malloc(sizeof(SDL_Rect));
    gSpriteClips[BOTTOM_RIGHT]->x = 100;
    gSpriteClips[BOTTOM_RIGHT]->y = 100;
    gSpriteClips[BOTTOM_RIGHT]->w = 100;
    gSpriteClips[BOTTOM_RIGHT]->h = 100;
}

void close() {
    gSpriteSheetTexture.free();

    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    IMG_Quit();
    SDL_Quit();
}

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
            initSpriteClips();
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
                
                //Render top left sprite
                gSpriteSheetTexture.render(0, 0, gSpriteClips[TOP_LEFT]);

                //Render top right sprite
                gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[TOP_RIGHT]->w, 0, gSpriteClips[TOP_RIGHT]);

                //Render bottom left sprite
                gSpriteSheetTexture.render(0, SCREEN_HEIGHT - gSpriteClips[BOTTOM_LEFT]->h, gSpriteClips[BOTTOM_LEFT]);

                //Render bottom right sprite
                gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[BOTTOM_RIGHT]->w, SCREEN_HEIGHT - gSpriteClips[BOTTOM_RIGHT]->h, gSpriteClips[BOTTOM_RIGHT]);

                //Update screen
                SDL_RenderPresent( gRenderer );
            }
        }
    }
    close();
    return 0;
} 

