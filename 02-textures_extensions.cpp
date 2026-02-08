#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <string>

//Screen dimension constants
constexpr int kScreenWidth{ 640 };
constexpr int kScreenHeight{ 480 };

class LTexture {
    public:
        LTexture();

        ~LTexture();

        bool loadFromFile(std::string path);

        void destroy();

        void render(float x, float y);

        int getWidth();
        int getHeight();
        bool isLoaded();

    private:
        SDL_Texture* mTexture;

        int mWidth;
        int mHeight;
};

//The window we'll be rendering to
SDL_Window* gWindow{ nullptr };

//The renderer used to draw to the window
SDL_Renderer* gRenderer{ nullptr };

//The PNG image we will render
LTexture gPngTexture;

LTexture::LTexture():
    mTexture{nullptr},
    mWidth{0},
    mHeight{0}
{}

LTexture::~LTexture() {
    destroy();
}

bool LTexture::loadFromFile(std::string path) {
    // Clean up texture if already exists
    destroy();

    // Load surface
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        SDL_Log("Unable to load image %s! SDL_image error: %s\n", path.c_str(), SDL_GetError());
    } else {
        // Create texture from surface
        mTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (mTexture == nullptr) {
            SDL_Log("Unable to create texture from loaded pixels! SDL error: %s\n", SDL_GetError());
        } else {
            // Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        // Clean up loaded surface
        SDL_DestroySurface(loadedSurface);
    }

    // Return success if texture loaded
    return mTexture != nullptr;
}

void LTexture::destroy() {
    SDL_DestroyTexture( mTexture );
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
}

void LTexture::render( float x, float y ) {
    //Set texture position
    SDL_FRect dstRect{x, y, static_cast<float>(mWidth), static_cast<float>(mHeight)};

    //Render texture
    SDL_RenderTexture(gRenderer, mTexture, nullptr, &dstRect);
}

int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeight() {
    return mHeight;
}

bool LTexture::isLoaded() {
    return mTexture != nullptr;
}

bool init() {
    bool success{true};

    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("SDL couldn't initialize! SDL error: %s\n", SDL_GetError());
    } else {
        // Create window with renderer
        if (
            SDL_CreateWindowAndRenderer(
                "SDL3 Tutorial: Textures and Extension Libraries",
                kScreenWidth,
                kScreenHeight,
                0,
                &gWindow,
                &gRenderer
            ) == false
        ) {
            SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
            success = false;
        }
    }

    return success;
}

bool loadMedia() {
    bool success{true};

    if(gPngTexture.loadFromFile("../loaded.png") == false) {
        SDL_Log("Unable to load png image!\n");
        success = false;
    }

    return success;
}

void close() {
    // Clean up texture
    gPngTexture.destroy();

    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    SDL_Quit();
}

int main(int argc, char* args[]) {
    int exitCode{0};

    // Initialize
    if (init() == false) {
        SDL_Log("Unable to initialize program!\n");
        exitCode = 1;
    } else {
        // Load media
        if (loadMedia() == false) {
            SDL_Log("Unable to load media!\n");
            exitCode = 2;
        } else {
            bool quit{false};

            // Event data
            SDL_Event e;
            SDL_zero(e);

            // Main loop
            while (quit == false) {
                while(SDL_PollEvent(&e) == true) {
                    switch(e.type) {
                        case SDL_EVENT_QUIT:
                            quit = true;
                            break;

                        case SDL_EVENT_KEY_DOWN:
                            SDL_Log("Keyboard key down!\n");
                            break;
                        case SDL_EVENT_KEY_UP:
                            SDL_Log("Keyboard key up!\n");
                            break;
                    }
                }

                // Fill background white
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                // Render image on screen
                gPngTexture.render(0.f, 0.f);

                // Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    close();

    return exitCode;
}