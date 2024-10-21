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
    std::vector<int> route; // Ruta a seguir
    int map_width; // Anchura del mapa
    GtkWidget* widget; // Widget para dibujar el proyectil

    Projectile(int start_x, int start_y, double dir_x, double dir_y, double spd, int map_width)
        : x(start_x), y(start_y), direction_x(dir_x), direction_y(dir_y), speed(spd), map_width(map_width), widget(nullptr) {}

    void update() {
        // Mover el proyectil a la siguiente posición de la ruta
        if (!route.empty()) {
            int next_pos = route.front();
            route.erase(route.begin()); // Eliminar el primer punto de la ruta
            x = next_pos / map_width;
            y = next_pos % map_width;
        } else {
            // Si no hay ruta, mover en la dirección normal
            x += static_cast<int>(direction_x * speed);
            y += static_cast<int>(direction_y * speed);
        }
    }
};

#endif // PROJECTILE_H
