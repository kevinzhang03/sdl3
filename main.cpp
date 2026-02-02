#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>

constexpr int kScreenWidth{ 640 };
constexpr int kScreenHeight{ 480 };

bool init();
bool loadMedia();
void close();

// The window we'll be rendering to
SDL_Window* gWindow{ nullptr };

// The surface contained by the window
SDL_Surface* gScreenSurface{ nullptr };

// The image we will load and show on the screen
SDL_Surface* gHelloWorld{ nullptr };


bool init() {
    // Init flag
    bool success{true};

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        // We're not using cout because of thread safety, and not using printf because of platform compatibility
        SDL_Log("SDL couldn't initialize! SDL error: %s\n", SDL_GetError());
        success = false;
    } else {
        // Create our window
        if ( // This is an init-statement, new in C++17! First assigns the variable, then checks the condition
            gWindow = SDL_CreateWindow("SDL3 Tutorial: Hello SDL3", kScreenWidth, kScreenHeight, 0);
            gWindow == nullptr
        ) {
            SDL_Log("Window couldn't be created! SDL error: %s\n", SDL_GetError());
            success = false;
        } else {
            // Get winow surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
}

bool loadMedia() {
    bool success{true};

    // Load splash image
    std::string imagePath{"sample.bmp"};

    if (
        gHelloWorld = SDL_LoadBMP(imagePath.c_str());
        gHelloWorld == nullptr
    ) {
        SDL_Log("Unable to load image %s! SDL error: %\n", imagePath.c_str(), SDL_GetError());
        success = false;
    }

    return success;
}

void close() {
    // Clean up surface
    SDL_DestroySurface(gHelloWorld);
    gHelloWorld = nullptr;
    
    // Destroy window
    // SDL_DestroyWindow also takes care of gScreenSurface
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gScreenSurface = nullptr;

    // Quit SDL subsystems
    SDL_Quit();
}

int main(int argc, char* args[]) {
    int exitCode{0};

    if (init() == false) {
        SDL_Log("Unable to init program!\n");
        exitCode = 1;
    } else {
        if(loadMedia() == false) {
            SDL_Log( "Unable to load media!\n" );
            exitCode = 2;
        } else {
            bool quit(false);

            // Event that can be a keypress, mouse movement, or when user exits the window
            SDL_Event e;
            SDL_zero(e); // Init the memory of e to zero

            // Main loop
            while (quit == false) {
                // Get event data
                while (SDL_PollEvent(&e) == true) {
                    if (e.type == SDL_EVENT_QUIT) {
                        quit = true;
                    }
                }

                // Fill surface white
                SDL_FillSurfaceRect(
                    gScreenSurface,
                    nullptr,
                    SDL_MapSurfaceRGB(gScreenSurface, 0xFF, 0xFF, 0xFF)
                );

                // Render image on screen
                SDL_BlitSurface(gHelloWorld, nullptr, gScreenSurface, nullptr);

                // Update surface
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }

    close();

    return exitCode;
}