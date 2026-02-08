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

# Find SDL3_image framework
find_library(SDL3_IMAGE_LIBRARY 
    NAMES SDL3_image
    PATHS /Library/Frameworks
    NO_DEFAULT_PATH
)

if(NOT SDL3_IMAGE_LIBRARY)
    message(FATAL_ERROR "SDL3_image framework not found in /Library/Frameworks")
endif()

# Glob all .cpp files
file(GLOB CPP_SOURCES "*.cpp")

# Create an executable for each .cpp file
foreach(SOURCE_FILE ${CPP_SOURCES})
    get_filename_component(EXEC_NAME ${SOURCE_FILE} NAME_WE)
    
    add_executable(${EXEC_NAME} ${SOURCE_FILE})
    target_link_libraries(${EXEC_NAME} ${SDL3_LIBRARY} ${SDL3_IMAGE_LIBRARY})
    
    set_target_properties(${EXEC_NAME} PROPERTIES
        BUILD_RPATH "/Library/Frameworks"
        INSTALL_RPATH "/Library/Frameworks;@executable_path/../Frameworks"
    )
endforeach()

# clang++ hello_sdl3.cpp -o out \
#   -F /Library/Frameworks \
#   -framework SDL3 \
#   -Wl,-rpath,/Library/Frameworks \
#   -Wl,-rpath,@executable_path/../Frameworks