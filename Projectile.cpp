// Projectile.cpp
#include "Projectile.h"
#include <cmath>
#include <iostream>

#include "GameLogic.h"  // Ahora puedes usar los métodos de GameLogic
#include "Tank.h"

Projectile::Projectile(double start_x, double start_y, double dir_x, double dir_y, double spd, int map_width, int map_height, GameLogic* logic, Tank* shooter)
    : x(start_x), y(start_y), direction_x(dir_x), direction_y(dir_y), speed(spd), map_width(map_width), map_height(map_height), game_logic(logic), rebotes(2), shooter(shooter) {
    // Constructor completo
}


void Projectile::update() {
    // Calcular la posición futura del proyectil
    // Ajusta las coordenadas para centrar el proyectil
    double future_x = x + direction_x * speed;
    double future_y = y + direction_y * speed;


    // Agregar la posición actual a las trazas antes de mover el proyectil
    game_logic->projectile_trail.emplace_back(x, y);

    // Verificar si el proyectil está fuera de los límites del mapa
    if (future_x < 0 || future_x >= map_width || future_y < 0 || future_y >= map_height) {
        // Si está fuera de los límites, marcarlo como inactivo para que desaparezca
        active = false;
        return;  // Salir de la función para evitar acceso a memoria inválida
    }

    // Convertir las coordenadas de píxeles del proyectil a coordenadas de celdas del mapa
    int cell_x = static_cast<int>(future_y / 25);  // Coordenadas de celda Y
    int cell_y = static_cast<int>(future_x / 25);  // Coordenadas de celda X

    // Comprobar si el proyectil está a punto de colisionar con un obstáculo
    if (game_logic->get_map()->has_obstacle(cell_x, cell_y)) {
        // Comprobar la colisión en todas las celdas adyacentes (incluyendo esquinas)
        for (int offset_x = -1; offset_x <= 1; ++offset_x) {
            for (int offset_y = -1; offset_y <= 1; ++offset_y) {
                int adj_cell_x = cell_x + offset_x;
                int adj_cell_y = cell_y + offset_y;

                // Verificar si esta celda es un obstáculo
                if (game_logic->get_map()->has_obstacle(adj_cell_x, adj_cell_y)) {
                    // Calcular las posiciones de los bordes del obstáculo
                    double obstacle_left = adj_cell_y * 25;
                    double obstacle_right = obstacle_left + 25;
                    double obstacle_top = adj_cell_x * 25;
                    double obstacle_bottom = obstacle_top + 25;

                    // Comprobar si colisiona
                    bool will_collide_horizontally = (x >= obstacle_left && x <= obstacle_right) &&
                                                     ((future_y <= obstacle_bottom && future_y >= obstacle_top) ||
                                                      (y >= obstacle_top && y <= obstacle_bottom));
                    bool will_collide_vertically = (y >= obstacle_top && y <= obstacle_bottom) &&
                                                   ((future_x <= obstacle_right && future_x >= obstacle_left) ||
                                                    (x >= obstacle_left && x <= obstacle_right));

                    // Manejo de colisiones con obstáculos
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
                        return;  // Salir de la función de actualización para evitar múltiples rebotes en un solo frame
                    }
                }
            }
        }
    }

    // Actualizar la posición del proyectil si no se ha desactivado
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
    const double collision_radius = 12.0;  // Ajustar el radio de colisión al tamaño del tanque

    for (Tank& tank : game_logic->get_tanks()) {
        if (!tank.is_active) continue;  // Ignorar tanques inactivos

        double distance = sqrt(pow(x - (tank.y * 25 + 12.5), 2) + pow(y - (tank.x * 25 + 12.5), 2));
        if (distance <= collision_radius) {
            double damage = 0.0;

            // Calcular el daño basado en el color del tanque impactado
            if (tank.color == "blue" || tank.color == "lightblue") {
                damage = 0.25 * tank.max_health;  // Daño de 25% a tanques celeste/azul
            } else if (tank.color == "red" || tank.color == "yellow") {
                damage = 0.50 * tank.max_health;  // Daño de 50% a tanques amarillo/rojo
            }

            // Reducir la salud del tanque impactado
            tank.health -= static_cast<int>(damage);
            if (tank.health < 0) {
                tank.health = 0;  // Asegurarse de que la salud no sea negativa
            }

            // Incrementar el daño total hecho por el tanque que disparó (shooter)
            shooter->total_damage_taken += static_cast<int>(damage);

            // **Verificar si el tanque ha sido destruido**
            if (tank.health == 0) {
                // Marcar el tanque como inactivo (destruido)
                tank.is_active = false;

                // **Marcar el tanque para ser eliminado**
                game_logic->mark_tank_for_removal(&tank);
            }

            // Redibujar el tanque impactado
            if (GTK_IS_WIDGET(tank.widget)) {
                gtk_widget_queue_draw(tank.widget);  // Redibujar el tanque impactado
            }

            // Redibujar el tanque que disparó
            if (GTK_IS_WIDGET(shooter->widget)) {
                gtk_widget_queue_draw(shooter->widget);  // Redibujar el tanque que disparó
            }

            // Marcar el proyectil para ser eliminado después de la colisión
            game_logic->mark_projectile_for_removal(this);
            return;
        }
    }
}






