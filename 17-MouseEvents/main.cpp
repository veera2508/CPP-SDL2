#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int BUTTON_HEIGHT = 200;
const int BUTTON_WIDTH = 300;
const int TOTAL_BUTTONS = 4;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

enum LButtonSprite {
    BUTTON_SPRITE_MOUSE_OUT,
    BUTTON_SPRITE_MOUSE_OVER_MOTION,
    BUTTON_SPRITE_MOUSE_DOWN,
    BUTTON_SPRITE_MOUSE_UP,
    BUTTON_SPRITE_TOTAL
};

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

        #if defined(SDL_TTF_MAJOR_VERSION)
        //Creates image from font string
        bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
        #endif

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

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor) {
    //Free the previous texture
    free();
    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
    if (textSurface == NULL)
        printf("Unable to render text surface! SDL Error: %s\n", TTF_GetError());
    else {
        mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (mTexture == NULL)
            printf("Cannot create texture! SDL Error: %s\n", SDL_GetError());
        else {
            //Set image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }
        SDL_FreeSurface(textSurface);
    }

    //Return success or failure
    return mTexture != NULL; 
}
#endif

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
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

LTexture gButtonSpriteSheetTexture;
SDL_Rect *gSpriteClips[BUTTON_SPRITE_TOTAL];

class LButton {
    public:
        //Initializes internal variables
        LButton();

        //Set top left position
        void setPosition( int x, int y );

        //Handles mouse event
        void handleEvent( SDL_Event* e );
    
        //Shows button sprite
        void render();

    private:
        //Top left position
        SDL_Point mPosition;

        //Currently used global sprite
        LButtonSprite mCurrentSprite;
};

LButton::LButton() {
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition(int x, int y) {
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e) {
    //If mouse event happend
    if ( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP ) {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );
        bool inside = false;

        //Check if mouse is inside button
        if (x >= mPosition.x && x <= mPosition.x + BUTTON_WIDTH && y >= mPosition.y && y <= mPosition.y + BUTTON_HEIGHT)
            inside = true;

        //Mouse is outside button
        if (!inside)
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;

        //Mouse is inside button
        else {
            //Set mouse over sprite
            switch( e->type ) {
                case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;

                case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                break;

                case SDL_MOUSEBUTTONUP:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                break;
            }
        }

    }
}

void LButton::render() {
    //Show current button sprite
    gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, gSpriteClips[ mCurrentSprite ] );
}

LButton gButtons[TOTAL_BUTTONS];

void initSpriteClips() {
    
    for (int i = 0; i < BUTTON_SPRITE_TOTAL; i++) {
        gSpriteClips[i] = (SDL_Rect*) malloc(sizeof(SDL_Rect));
        gSpriteClips[i]->x = 0;
        gSpriteClips[i]->y = i*200;
        gSpriteClips[i]->w = BUTTON_WIDTH;
        gSpriteClips[i]->h = BUTTON_HEIGHT;
    }
}

void initButtonLocations() {
    //Set button locations
    gButtons[0].setPosition(0, 0);
    gButtons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
    gButtons[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);
    gButtons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
}

bool loadMedia() {
    bool success = true;

    if (!gButtonSpriteSheetTexture.loadFromFile("button.png")) {
        printf("Error importing texture!");
        success = false;
    }

    return success;
}

void close() {
    
    gButtonSpriteSheetTexture.free();

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
                
                #if defined(SDL_TTF_MAJOR_VERSION)
                if (TTF_Init() == -1) {
                    printf("SDL_ttf could not be initialized! Error: %s\n", TTF_GetError());
                    success = false;
                }
                #endif
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
            initSpriteClips();
            initButtonLocations();
            SDL_Event e; //Variable to Store Event
            //Main Loop
            while (!quit) {
                //Loop to get events from event queue
                while (SDL_PollEvent( &e ) != 0) {
                    //User requests quit
                    if( e.type == SDL_QUIT ) {
                        quit = true;
                    }

                    for (int i = 0; i < TOTAL_BUTTONS; i++) 
                        gButtons[i].handleEvent( &e );
                }
                 //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                //Clear Screen
                SDL_RenderClear( gRenderer );
                
                //Render the texture
                for (int i = 0; i < TOTAL_BUTTONS; i++)
                    gButtons[i].render();

                //Update screen
                SDL_RenderPresent( gRenderer );
            }
        }
    }
    close();
    return 0;
} 