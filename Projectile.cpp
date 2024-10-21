// Projectile.cpp
#include "Projectile.h"
#include <cmath>

void Projectile::update() {
    // Actualizar la posición del proyectil en píxeles según la dirección y velocidad
    x += direction_x * speed;
    y += direction_y * speed;

    // Verificar si el proyectil ha salido del área de juego (en píxeles)
    if (x < 0 || x >= map_width || y < 0 || y >= map_width) {
        active = false;  // Desactivar el proyectil si está fuera del mapa
        return;
    }
}
