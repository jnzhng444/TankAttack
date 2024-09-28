#include "GameLogic.h"
#include "Pathfinding.h"  // Incluir el archivo de pathfinding
#include <cstdlib>
#include <ctime>
#include <iostream>

GameLogic::GameLogic(int num_tanks_per_player, Map* map)
    : num_tanks_per_player(num_tanks_per_player), map(map) {
    std::srand(std::time(0));  // Inicializar el generador aleatorio
}

void GameLogic::generate_tanks() {
    for (int player = 1; player <= 2; ++player) {
        for (int i = 0; i < 2; ++i) {  // Solo 2 tanques por jugador
            Tank tank;
            tank.id = player * 10 + i;  // ID único para cada tanque
            tank.player = player;
            tank.game_logic = this;  // Asignar puntero a game_logic
            tank.widget = nullptr;  // Asignar widget más adelante cuando se pase el área de juego

            // Definir color del tanque basado en el jugador
            if (player == 1) {
                tank.color = (i == 0) ? "blue" : "red";
            } else {
                tank.color = (i == 0) ? "lightblue" : "yellow";
            }

            // Encontrar una posición accesible para el tanque
            int x, y;
            do {
                x = std::rand() % map->get_width();  // Usar get_width()
                y = std::rand() % map->get_height();  // Usar get_height()
            } while (!map->is_accessible(x, y) || map->has_obstacle(x, y));

            tank.x = x;
            tank.y = y;
            tanks.push_back(tank);  // Agregar el tanque al vector
        }
    }
}

void GameLogic::move_tank(int tank_id, int x, int y) {
    for (Tank& tank : tanks) {
        if (tank.id == tank_id) {
            // Verificar si la nueva posición es válida (sin obstáculos)
            if (x >= 0 && x < map->get_width() && y >= 0 && y < map->get_height() && !map->has_obstacle(x, y)) {
                tank.x = x;
                tank.y = y;
                std::cout << "Tanque " << tank_id << " movido a (" << x << ", " << y << ")" << std::endl;

                // Verificar que el widget es válido antes de redibujar
                if (GTK_IS_WIDGET(tank.widget)) {
                    gtk_widget_queue_draw(tank.widget);  // Redibujar el área de juego
                }
            } else {
                std::cout << "Movimiento inválido." << std::endl;
            }
            break;
        }
    }
}

void GameLogic::calculate_route(Tank& tank, int target_x, int target_y) {
    // Borrar la ruta anterior
    current_route.clear();

    int prob = std::rand() % 100;

    if (tank.color == "blue" || tank.color == "lightblue") {
        if (prob < 50) {
            std::vector<int> path = Pathfinding::bfs(map, tank.x, tank.y, target_x, target_y);
            if (!path.empty()) {
                tank.route = path;
                current_route = path;  // Guardar la ruta actual
                g_timeout_add(100, move_tank_step_by_step, &tank);
            }
        } else {
            random_movement_with_los(tank, target_x, target_y);
        }
    } else if (tank.color == "red" || tank.color == "yellow") {
        if (prob < 80) {
            std::vector<int> path = Pathfinding::dijkstra(map, tank.x, tank.y, target_x, target_y);
            if (!path.empty()) {
                tank.route = path;
                current_route = path;  // Guardar la ruta actual
                g_timeout_add(100, move_tank_step_by_step, &tank);
            }
        } else {
            random_movement_with_los(tank, target_x, target_y);
        }
    }
}

gboolean GameLogic::move_tank_step_by_step(gpointer data) {
    Tank* tank = static_cast<Tank*>(data);

    if (!tank->route.empty()) {
        int next_pos = tank->route.front();
        tank->route.erase(tank->route.begin());  // Eliminar el primer punto de la ruta
        int new_x = next_pos / tank->game_logic->map->get_width();
        int new_y = next_pos % tank->game_logic->map->get_width();

        // Mover el tanque a la nueva posición
        tank->game_logic->move_tank(tank->id, new_x, new_y);

        // Forzar el redibujado solo si el widget es válido
        if (GTK_IS_WIDGET(tank->widget)) {
            gtk_widget_queue_draw(tank->widget);  // Redibujar el área de dibujo
        } else {
            std::cout << "Widget no válido para redibujar." << std::endl;
        }

        return TRUE;  // Mantener el temporizador activo hasta que la ruta esté vacía
    }

    return FALSE;  // Detener el temporizador cuando no haya más movimientos
}

void GameLogic::random_movement_with_los(Tank& tank, int target_x, int target_y) {
    int new_x, new_y;
    bool moved = false;

    // Intentar dos veces encontrar una posición sin obstáculos en un radio de 2 casillas
    for (int attempt = 0; attempt < 2; ++attempt) {
        int dx = std::rand() % 5 - 2;  // Movimiento aleatorio en X (-2 a +2)
        int dy = std::rand() % 5 - 2;  // Movimiento aleatorio en Y (-2 a +2)

        new_x = tank.x + dx;
        new_y = tank.y + dy;

        // Verificar que la nueva posición está dentro del mapa y sin obstáculos
        if (new_x >= 0 && new_x < map->get_width() && new_y >= 0 && new_y < map->get_height() && !map->has_obstacle(new_x, new_y)) {
            move_tank(tank.id, new_x, new_y);
            moved = true;
            std::cout << "Movimiento aleatorio: Tanque movido a (" << new_x << ", " << new_y << ")" << std::endl;

            // Forzar el redibujado si el widget es válido
            if (GTK_IS_WIDGET(tank.widget)) {
                gtk_widget_queue_draw(tank.widget);  // Redibujar el área de juego
            }

            break;  // Si se movió correctamente, salir del bucle
        }
    }

    // Si después de dos intentos no se pudo mover, avanzar lo más posible sin romper la línea de vista
    if (!moved) {
        std::cout << "No se pudo mover por línea de vista, avanzando lo más posible." << std::endl;

        // Aquí se puede aplicar una lógica para avanzar sin romper la línea de vista
        if (target_x > tank.x) {
            move_tank(tank.id, tank.x + 1, tank.y);
        } else if (target_x < tank.x) {
            move_tank(tank.id, tank.x - 1, tank.y);
        } else if (target_y > tank.y) {
            move_tank(tank.id, tank.x, tank.y + 1);
        } else if (target_y < tank.y) {
            move_tank(tank.id, tank.x, tank.y - 1);
        }

        // Forzar el redibujado si el widget es válido
        if (GTK_IS_WIDGET(tank.widget)) {
            gtk_widget_queue_draw(tank.widget);  // Redibujar el área de juego
        }
    }
}

std::vector<Tank>& GameLogic::get_tanks() {
    return tanks;  // Devolver referencia no constante
}

Map* GameLogic::get_map() const {
    return map;
}
