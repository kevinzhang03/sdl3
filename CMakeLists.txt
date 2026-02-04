cmake_minimum_required(VERSION 3.20)
project(helloSDL3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL3 framework
find_library(SDL3_LIBRARY 
    NAMES SDL3
    PATHS /Library/Frameworks
    NO_DEFAULT_PATH
)

if(NOT SDL3_LIBRARY)
    message(FATAL_ERROR "SDL3 framework not found in /Library/Frameworks")
endif()

# Create executable
add_executable(helloSDL3 hello_sdl3.cpp)

# Link SDL3 framework
target_link_libraries(helloSDL3 ${SDL3_LIBRARY})

# Set rpath for runtime library loading
set_target_properties(helloSDL3 PROPERTIES
    BUILD_RPATH "/Library/Frameworks"
    INSTALL_RPATH "/Library/Frameworks;@executable_path/../Frameworks"
)

# clang++ hello_sdl3.cpp -o out \
#   -F /Library/Frameworks \
#   -framework SDL3 \
#   -Wl,-rpath,/Library/Frameworks \
#   -Wl,-rpath,@executable_path/../Frameworks