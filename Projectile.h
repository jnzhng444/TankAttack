// Projectile.h
#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Map.h"
#include <gtk/gtk.h>

class Projectile {
public:
    double x, y;  // Usamos double para mayor precisión en la dirección
    double direction_x, direction_y; // Dirección normalizada del proyectil
    double speed;  // Velocidad del proyectil
    int map_width; // Anchura del mapa
    GtkWidget* widget;  // Widget para el proyectil
    bool active = true;

    // Constructor
    Projectile(double start_x, double start_y, double dir_x, double dir_y, double spd, int map_width)
    : x(start_x), y(start_y), direction_x(dir_x), direction_y(dir_y), speed(spd), map_width(map_width), widget(nullptr) {}

    // Método para actualizar la posición del proyectil
    void update();
};

#endif // PROJECTILE_H
