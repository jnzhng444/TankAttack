// Projectile.cpp
#include "Projectile.h"
#include <cmath>
#include "GameLogic.h"  // Ahora puedes usar los métodos de GameLogic
#include "Tank.h"

Projectile::Projectile(double start_x, double start_y, double dir_x, double dir_y, double spd, int map_width, int map_height, GameLogic* logic)
    : x(start_x), y(start_y), direction_x(dir_x), direction_y(dir_y), speed(spd), map_width(map_width), map_height(map_height), game_logic(logic), rebotes(2) {
    // Constructor completo
}

void Projectile::update() {
    // Calcular la posición futura del proyectil
    double future_x = x + direction_x * speed;
    double future_y = y + direction_y * speed;

    // Convertir las coordenadas de píxeles del proyectil a coordenadas de celdas del mapa
    int cell_x = static_cast<int>(future_y / 25);  // Coordenadas de celda Y
    int cell_y = static_cast<int>(future_x / 25);  // Coordenadas de celda X

    // Comprobar si el proyectil está a punto de colisionar con un obstáculo
    if (game_logic->get_map()->has_obstacle(cell_x, cell_y)) {
        // Calcular las posiciones de los bordes del obstáculo
        double obstacle_left = cell_y * 25;
        double obstacle_right = obstacle_left + 25;
        double obstacle_top = cell_x * 25;
        double obstacle_bottom = obstacle_top + 25;

        // Comprobar si colisiona
        bool will_collide_horizontally = (x >= obstacle_left && x <= obstacle_right) &&
                                         ((future_y <= obstacle_bottom && future_y >= obstacle_top) ||
                                          (y >= obstacle_top && y <= obstacle_bottom));
        bool will_collide_vertically = (y >= obstacle_top && y <= obstacle_bottom) &&
                                       ((future_x <= obstacle_right && future_x >= obstacle_left) ||
                                        (x >= obstacle_left && x <= obstacle_right));

        // Manejo de rebotes
        if (will_collide_horizontally || will_collide_vertically) {
            // Ajustar rebote y mover el proyectil fuera del obstáculo
            if (will_collide_horizontally) {
                direction_y = -direction_y;  // Rebote vertical
                y = direction_y > 0 ? obstacle_bottom + 0.1 : obstacle_top - 0.1;  // Ajustar posición
            }

            if (will_collide_vertically) {
                direction_x = -direction_x;  // Rebote horizontal
                x = direction_x > 0 ? obstacle_right + 0.1 : obstacle_left - 0.1;  // Ajustar posición
            }

            rebotes--;  // Reducir el número de rebotes permitidos

            // Asegurarse de que no se mueva en caso de colisión
            future_x = x;
            future_y = y;
        }
    }

    // Rebote en las paredes del mapa
    if (future_x < 0 || future_x >= map_width) {
        direction_x = -direction_x;  // Rebote horizontal en las paredes
        rebotes--;
        future_x = x;  // Asegurarse de que no se mueva en caso de colisión
    }

    if (future_y < 0 || future_y >= map_height) {
        direction_y = -direction_y;  // Rebote vertical en las paredes
        rebotes--;
        future_y = y;  // Asegurarse de que no se mueva en caso de colisión
    }

    // Mover el proyectil solo si no se desactivó por falta de rebotes
    if (rebotes > 0) {
        x = future_x;
        y = future_y;
    } else {
        active = false;
    }

    // Verificar colisiones con tanques
    handle_collision();
}




void Projectile::handle_collision() {
    const double collision_radius = 12.0;  // Ajustar el radio de colisión al tamaño del tanque (en píxeles)

    for (auto it = game_logic->get_tanks().begin(); it != game_logic->get_tanks().end();) {
        // Verificar si el tanque está activo antes de calcular la colisión
        if (!it->is_active) {
            ++it;
            continue;  // Saltar tanques inactivos
        }

        // Calcular la distancia entre el proyectil y el tanque
        double distance = sqrt(pow(x - (it->y * 25 + 12.5), 2) + pow(y - (it->x * 25 + 12.5), 2));

        if (distance <= collision_radius) {  // Si la distancia es menor o igual al radio de colisión
            // Aplicar daño
            if (it->color == "blue" || it->color == "lightblue") {
                it->health -= 0.25 * it->max_health;  // Daño de 25%
            } else if (it->color == "red" || it->color == "yellow") {
                it->health -= 0.50 * it->max_health;  // Daño de 50%
            }

            // Comprobar si el tanque ha sido destruido
            if (it->is_destroyed()) {
                game_logic->remove_tank(*it);  // Llamar a la función para eliminar el tanque
                it = game_logic->get_tanks().erase(it);  // Eliminar el tanque de la lista
            } else {
                ++it;  // Solo avanza si no se eliminó el tanque
            }

            // Desactivar el proyectil
            active = false;
            return;  // Salir después de aplicar daño
        } else {
            ++it;  // Avanzar al siguiente tanque
        }
    }
}

