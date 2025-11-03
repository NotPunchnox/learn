# Version minimale de Cmake requise
cmake_minimum_required(VERSION 3.16)

# Nom du projet
project(Simulateur-de-ZOO-POO LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Forcer la build OBLIGATOIREMENT en mode Release
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Release")

# Configuration spécifique pour MSVC
if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Configuration des chemins pour MySQL Connector C++ ( windows )
if(WIN32)
    set(MYSQL_CONNECTOR_PATH "C:\\Program Files\\MySQL\\MySQL Connector C++ 9.5")
    set(MYSQL_INC "${MYSQL_CONNECTOR_PATH}\\include")
    set(MYSQL_LIB "${MYSQL_CONNECTOR_PATH}\\lib64")
    set(MYSQL_LIB "${MYSQL_CONNECTOR_PATH}\\lib64\\vs14")
    
    if(EXISTS "${MYSQL_INC}")
        include_directories("${MYSQL_INC}")
        if(EXISTS "${MYSQL_LIB}")
            link_directories("${MYSQL_LIB}")
        endif()
    else()
        message(FATAL_ERROR "MySQL Connector C++ not found at ${MYSQL_INC}")
    endif()

else ()
  set(MYSQL_CONNECTOR_PATH "/home/punchnox/Documents/C++/mysql-connector-c++-9.5.0-linux-glibc2.28-x86-64bit")
  set(MYSQL_INC "${MYSQL_CONNECTOR_PATH}/include")
  set(MYSQL_LIB "${MYSQL_CONNECTOR_PATH}/lib64")

  if(EXISTS "${MYSQL_INC}")
      include_directories("${MYSQL_INC}")
      if(EXISTS "${MYSQL_LIB}")
          link_directories("${MYSQL_LIB}")
      endif()
  else()
      message(FATAL_ERROR "MySQL Connector C++ not found at ${MYSQL_INC}")
  endif()
endif()

# Ajout des fichiers sources
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
    src/dao/EmployeDao.cpp
    src/vue/afficher.cpp
)

# Inclusion des répertoires d'en-têtes
target_include_directories(ZOO PRIVATE include)
target_link_libraries(ZOO PRIVATE mysqlcppconn mysqlcppconnx)
