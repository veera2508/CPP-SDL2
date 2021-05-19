#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int WALKING_ANIMATION_FRAMES = 4;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Rect* gSpriteClips[ WALKING_ANIMATION_FRAMES ];

class LTexture {
    public:
        //ConstructorColor Modulation of textures
        LTexture();

        //Destructor
        ~LTexture();

        //Load image into texture
        bool loadFromFile( std::string path );

        //Dealocates texture
        void free();

        //Renders texture at a given point
        void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

        //Set color modulation
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        //Set blending
        void setBlendMode( SDL_BlendMode blending );

        //set alpha modulation
        void setAlpha( Uint8 alpha );

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

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip){
    SDL_Rect renderquad = { x, y, mWidth, mHeight };
    //Set clip rendering dimensions
    if (clip != NULL) {
        renderquad.w = clip->w;
        renderquad.h = clip->h;
    }
    //Render to screen
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderquad, angle, center, flip);
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue ) {   
    //Modulate Texture
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending ) {
    //Set blending function
    SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha ) {
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

int LTexture::getHeight() {
    return mHeight;
}

int LTexture::getWidth() {
    return mWidth;
}

LTexture gArrowTexture;

bool loadMedia() {
    bool success = true;

    //Load Foo texture
    if ( !gArrowTexture.loadFromFile("arrow.png") ) {
        printf("Failed to load sprite sheet!\n");
        success = false;
    }
    return success;
}

void close() {
    gArrowTexture.free();

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
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
            double degrees = 0;
            SDL_RendererFlip flipType = SDL_FLIP_NONE;
            //Main Loop
            while (!quit) {
                //Loop to get events from event queue
                while (SDL_PollEvent( &e ) != 0) {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    else if( e.type == SDL_KEYDOWN )
                    {
                        switch( e.key.keysym.sym )
                        {
                            case SDLK_a:
                            degrees -= 60;
                            break;
                            
                            case SDLK_d:
                            degrees += 60;
                            break;

                            case SDLK_q:
                            flipType = SDL_FLIP_HORIZONTAL;
                            break;

                            case SDLK_w:
                            flipType = SDL_FLIP_NONE;
                            break;

                            case SDLK_e:
                            flipType = SDL_FLIP_VERTICAL;
                            break;
                        }
                    }
                    

                }
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                //Clear Screen
                SDL_RenderClear( gRenderer );
                
                //Render the texture
                gArrowTexture.render((SCREEN_WIDTH - gArrowTexture.getWidth())/2, (SCREEN_HEIGHT - gArrowTexture.getHeight())/2, NULL, degrees, NULL, flipType);
                
                //Update screen
                SDL_RenderPresent( gRenderer );
            }
        }
    }
    close();
    return 0;
} 