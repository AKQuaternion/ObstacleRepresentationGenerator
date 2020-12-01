cmake_minimum_required(VERSION 3.17)
project(ObstacleRepresentationGenerator)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ObstacleRepresentationGenerator
        Bracelet.cpp
        Bracelet.hpp
        DodecahedronFunctions.cpp
        DodecahedronFunctions.hpp
        main.cpp)
