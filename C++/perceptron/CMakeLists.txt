# Version minimale de Cmake requise
cmake_minimum_required(VERSION 3.16)

# Nom du projet
project(Perceptron_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES_FILES main.cpp)

# Ajout des fichiers sources
add_executable(Perceptron_cpp main.cpp)

# Inclusion des répertoires d'en-têtes
#target_include_directories(ZOO PRIVATE include)
