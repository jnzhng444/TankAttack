// Projectile.cpp
#include "Projectile.h"
#include <cmath>
#include "GameLogic.h"  // Ahora puedes usar los métodos de GameLogic
#include "Tank.h"

Projectile::Projectile(double start_x, double start_y, double dir_x, double dir_y, double spd, int map_width, int map_height, GameLogic* logic)
    : x(start_x), y(start_y), direction_x(dir_x), direction_y(dir_y), speed(spd), map_width(map_width), map_height(map_height), game_logic(logic), rebotes(3) {
    // Constructor completo
}

void Projectile::update() {
    // Mover el proyectil en la dirección calculada
    x += direction_x * speed;
    y += direction_y * speed;

    // Verificar si toca las paredes del mapa
    if (x < 0 || x >= map_width) {
        direction_x = -direction_x;  // Rebote horizontal
        rebotes--;
    }

    if (y < 0 || y >= map_height) {
        direction_y = -direction_y;  // Rebote vertical
        rebotes--;
    }

    // Si ya no tiene más rebotes permitidos, desactivar el proyectil
    if (rebotes <= 0) {
        active = false;
        return;
    }

    // Verificar colisiones con tanques
    handle_collision();
}

void Projectile::handle_collision() {
    const double collision_radius = 12.5;  // Radio de colisión (ajústalo según sea necesario)

    for (Tank& tank : game_logic->get_tanks()) {
        // Calcular la distancia entre el proyectil y el tanque
        double distance = sqrt(pow(x - (tank.y * 25 + 12.5), 2) + pow(y - (tank.x * 25 + 12.5), 2));

        if (distance <= collision_radius) {  // Si la distancia es menor o igual al radio de colisión
            // Aplicar daño
            if (tank.color == "blue" || tank.color == "lightblue") {
                tank.health -= 0.25 * tank.max_health;  // Daño de 25%
            } else if (tank.color == "red" || tank.color == "yellow") {
                tank.health -= 0.50 * tank.max_health;  // Daño de 50%
            }

            // Desactivar el proyectil
            active = false;
            return;
        }
    }
}
