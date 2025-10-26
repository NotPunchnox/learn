# Version minimale de Cmake requise
cmake_minimum_required(VERSION 3.16)

# Nom du projet
project(Simulateur-de-ZOO-POO LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(ZOO
    main.cpp
    src/models/Animal.cpp
)

target_include_directories(ZOO PRIVATE include)