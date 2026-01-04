cmake_minimum_required(VERSION 3.16)
project(PhysicsSim LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML 3
find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio Window System)

# Main executable
add_executable(PhysicsSim
        src/main.cpp
        src/body.h
        src/include.h
        src/definitions.h
        src/utils.h
        src/ui.h
)

# tinyfiledialogs library (C, not C++)
add_library(tfd
        src/tfd/tinyfiledialogs.c
)

target_include_directories(tfd PUBLIC
        src/tfd
)

# Link everything
target_link_libraries(PhysicsSim PRIVATE
        SFML::Graphics
        SFML::Audio
        SFML::Window
        SFML::System
        tfd
)

# macOS frameworks needed by tinyfiledialogs
if (APPLE)
    target_link_libraries(PhysicsSim PRIVATE
            "-framework Cocoa"
    )
endif()
