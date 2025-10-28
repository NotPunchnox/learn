# Version minimale de Cmake requise
cmake_minimum_required(VERSION 3.16)

# Nom du projet
project(Simulateur-de-ZOO-POO LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
    set(MYSQL_CONNECTOR_PATH "C:\\Program Files\\MySQL\\MySQL Connector C++ 9.5")
    set(MYSQL_INC "${MYSQL_CONNECTOR_PATH}\\include")
    set(MYSQL_LIB "${MYSQL_CONNECTOR_PATH}\\lib64")
    set(MYSQL_LIB_VS14 "${MYSQL_CONNECTOR_PATH}\\lib64\\vs14")
    
    if(EXISTS "${MYSQL_INC}")
        include_directories("${MYSQL_INC}")
        if(EXISTS "${MYSQL_LIB_VS14}")
            link_directories("${MYSQL_LIB_VS14}")
        endif()
    else()
        message(FATAL_ERROR "MySQL Connector C++ not found at ${MYSQL_INC}")
    endif()

endif()

add_executable(ZOO
    main.cpp
    src/models/Animal.cpp
    src/models/Mammifere.cpp
    src/models/Oiseau.cpp
    src/models/Reptile.cpp
    src/models/Enclos.cpp
    src/models/Employe.cpp
    src/models/Zoo.cpp
    src/utils/DatabaseManager.cpp
    src/dao/AnimalDao.cpp
)

target_include_directories(ZOO PRIVATE include)
target_link_libraries(ZOO PRIVATE mysqlcppconnx libssl libcrypto)