cmake_minimum_required(VERSION 3.29)
project(TankAttack)

# Establece el estándar de C++
set(CMAKE_CXX_STANDARD 20)

# Busca y encuentra las bibliotecas necesarias de GTK usando pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Incluye los directorios de las bibliotecas de GTK
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

# Añade las banderas de compilación y enlazado de GTK
add_definitions(${GTK3_CFLAGS_OTHER})

# Compila el ejecutable del proyecto TankAttack
add_executable(TankAttack main.cpp
        GameWindow.cpp
        GameWindow.h
        GameArea.cpp
        GameArea.h
        GameLogic.cpp
        GameLogic.h
        Map.cpp
        Map.h
        Pathfinding.cpp
        Pathfinding.h
        Tank.h
        Projectile.h)

# Enlaza las bibliotecas de GTK3 con el ejecutable
target_link_libraries(TankAttack ${GTK3_LIBRARIES})
