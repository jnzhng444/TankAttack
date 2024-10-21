// Projectile.cpp

#include "Projectile.h"

#include <cmath>

// Implementación del método update para mover el proyectil
void Projectile::update() {
    // Mover el proyectil en la dirección calculada con la velocidad asignada
    x += direction_x * speed;
    y += direction_y * speed;

    // Redondear las coordenadas para mantenerlas en enteros válidos
    x = std::round(x);
    y = std::round(y);

    // Verificar si las nuevas coordenadas están dentro de los límites del mapa antes de cualquier otro acceso
    if (x < 0 || x >= map_width || y < 0 || y >= map_width) {
        active = false;  // Marcamos el proyectil como inactivo si sale del mapa
        return;  // Terminar la función si el proyectil está fuera de los límites
    }
}
