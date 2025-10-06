cmake_minimum_required(VERSION 4.0)
project(PhysicsSim)

set(CMAKE_CXX_STANDARD 20)
find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio Window System)
add_executable(PhysicsSim src/main.cpp
        src/body.h
        src/include.h
        src/definitions.h
)
target_link_libraries(PhysicsSim PRIVATE
        SFML::Graphics
        SFML::Audio
        SFML::Window
        SFML::System
)
