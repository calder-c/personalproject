cmake_minimum_required(VERSION 4.0)
project(PhysicsSim)

set(CMAKE_CXX_STANDARD 20)

add_executable(PhysicsSim src/main.cpp
        src/body.h
        src/include.h
        src/definitions.h)
