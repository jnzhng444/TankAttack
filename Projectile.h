// Projectile.h
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Map.h"
#include <gtk/gtk.h>  // Añadir para usar GtkWidget

class Projectile {
public:
    int x, y; // Posición del proyectil
    double direction_x, direction_y; // Dirección del proyectil
    double speed; // Velocidad del proyectil
    int map_width; // Anchura del mapa
    GtkWidget* widget; // Widget para dibujar el proyectil
    bool active = true;

    Projectile(int start_x, int start_y, double dir_x, double dir_y, double spd, int map_width)
    : x(start_x), y(start_y), direction_x(dir_x), direction_y(dir_y), speed(spd), map_width(map_width), widget(nullptr) {}


    // Declaración del método update (la implementación irá en Projectile.cpp)
    void update();
};

#endif // PROJECTILE_H
