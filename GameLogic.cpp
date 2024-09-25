#include "GameLogic.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <cmath>  // Para std::abs
#include <iostream>
#include <glib.h>
#include <gtk/gtk.h>
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
            } else {
                std::cout << "Movimiento inválido." << std::endl;
            }
            break;
        }
    }
}



void GameLogic::calculate_route(Tank& tank, int target_x, int target_y) {
    int prob = std::rand() % 100;

    if (tank.color == "blue" || tank.color == "lightblue") {
        if (prob < 50) {
            std::vector<int> path = bfs(tank.x, tank.y, target_x, target_y);
            if (!path.empty()) {
                tank.route = path;
                g_timeout_add(100, move_tank_step_by_step, &tank);  // Iniciar el temporizador con el tanque
            }
        } else {
            random_movement_with_los(tank, target_x, target_y);
        }
    } else if (tank.color == "red" || tank.color == "yellow") {
        if (prob < 80) {
            std::vector<int> path = dijkstra(tank.x, tank.y, target_x, target_y);
            if (!path.empty()) {
                tank.route = path;
                g_timeout_add(100, move_tank_step_by_step, &tank);  // Iniciar el temporizador con el tanque
            }
        } else {
            random_movement_with_los(tank, target_x, target_y);
        }
    }
}


// Función que mueve el tanque un paso a lo largo de su ruta
gboolean GameLogic::move_tank_step_by_step(gpointer data) {
    Tank* tank = static_cast<Tank*>(data);

    if (!tank->route.empty()) {
        int next_pos = tank->route.front();
        tank->route.erase(tank->route.begin());  // Eliminar el primer punto de la ruta
        int new_x = next_pos / tank->game_logic->map->get_width();
        int new_y = next_pos % tank->game_logic->map->get_width();

        // Mover el tanque a la nueva posición
        tank->game_logic->move_tank(tank->id, new_x, new_y);

        // Verificar que el widget es válido antes de redibujar
        if (GTK_IS_WIDGET(tank->widget)) {
            gtk_widget_queue_draw(tank->widget);  // Redibujar el área de dibujo
        } else {
            std::cout << "Widget no válido para redibujar" << std::endl;
        }

        return TRUE;  // Mantener el temporizador activo hasta que la ruta esté vacía
    }

    return FALSE;  // Detener el temporizador cuando no haya más movimientos
}




void GameLogic::random_movement_with_los(Tank& tank, int target_x, int target_y) {
    int new_x, new_y;
    bool moved = false;

    // Intentar dos veces encontrar una posición sin obstáculos en un radio definido
    for (int attempt = 0; attempt < 2; ++attempt) {
        new_x = tank.x + (std::rand() % 3 - 1);  // Movimiento aleatorio en X (-1, 0, +1)
        new_y = tank.y + (std::rand() % 3 - 1);  // Movimiento aleatorio en Y (-1, 0, +1)

        if (new_x >= 0 && new_x < map->get_width() && new_y >= 0 && new_y < map->get_height() && !map->has_obstacle(new_x, new_y)) {
            move_tank(tank.id, new_x, new_y);
            moved = true;
            break;
        }
    }

    if (!moved) {
        std::cout << "No se pudo mover por línea de vista." << std::endl;
    }
}

std::vector<int> GameLogic::bfs(int start_x, int start_y, int goal_x, int goal_y) {
    int width = map->get_width();
    int height = map->get_height();

    // Crear una matriz de distancias y una matriz para registrar el predecesor de cada celda
    std::vector<std::vector<int>> dist(width, std::vector<int>(height, -1));
    std::vector<std::vector<int>> prev(width, std::vector<int>(height, -1));

    // Cola para BFS y un vector de direcciones de movimiento (arriba, abajo, izquierda, derecha)
    std::queue<std::pair<int, int>> q;
    std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    // Inicializar BFS desde el punto de partida
    q.push({start_x, start_y});
    dist[start_x][start_y] = 0;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        // Si llegamos al objetivo, reconstruimos el camino
        if (x == goal_x && y == goal_y) {
            std::vector<int> path;
            while (x != start_x || y != start_y) {
                path.push_back(x * width + y);
                int p = prev[x][y];
                x = p / width;
                y = p % width;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Explorar los vecinos
        for (const auto& [dx, dy] : directions) {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && ny >= 0 && nx < width && ny < height && dist[nx][ny] == -1 && !map->has_obstacle(nx, ny)) {
                q.push({nx, ny});
                dist[nx][ny] = dist[x][y] + 1;
                prev[nx][ny] = x * width + y;
            }
        }
    }

    // Si no hay un camino válido, retornar una ruta vacía
    return {};
}

#include <queue>
#include <vector>
#include <limits>

std::vector<int> GameLogic::dijkstra(int start_x, int start_y, int goal_x, int goal_y) {
    int width = map->get_width();
    int height = map->get_height();

    // Crear una matriz de distancias y una matriz para registrar el predecesor de cada celda
    std::vector<std::vector<int>> dist(width, std::vector<int>(height, std::numeric_limits<int>::max()));
    std::vector<std::vector<int>> prev(width, std::vector<int>(height, -1));

    // Cola de prioridad para Dijkstra, la cola almacena pares (distancia, {x, y})
    std::priority_queue<std::pair<int, std::pair<int, int>>, std::vector<std::pair<int, std::pair<int, int>>>, std::greater<>> pq;

    // Direcciones de movimiento (arriba, abajo, izquierda, derecha)
    std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    // Inicializar Dijkstra desde el punto de partida
    pq.push({0, {start_x, start_y}});
    dist[start_x][start_y] = 0;

    while (!pq.empty()) {
        auto [current_dist, pos] = pq.top();
        int x = pos.first, y = pos.second;
        pq.pop();

        // Si llegamos al objetivo, reconstruimos el camino
        if (x == goal_x && y == goal_y) {
            std::vector<int> path;
            while (x != start_x || y != start_y) {
                path.push_back(x * width + y);
                int p = prev[x][y];
                x = p / width;
                y = p % width;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Si ya encontramos una mejor distancia para este nodo, continuamos
        if (current_dist > dist[x][y]) {
            continue;
        }

        // Explorar los vecinos
        for (const auto& [dx, dy] : directions) {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && ny >= 0 && nx < width && ny < height && !map->has_obstacle(nx, ny)) {
                int new_dist = current_dist + 1;  // El peso de cada movimiento es 1 (uniforme)
                if (new_dist < dist[nx][ny]) {
                    dist[nx][ny] = new_dist;
                    prev[nx][ny] = x * width + y;
                    pq.push({new_dist, {nx, ny}});
                }
            }
        }
    }

    // Si no hay un camino válido, retornar una ruta vacía
    return {};
}




std::vector<Tank>& GameLogic::get_tanks() {
    return tanks;  // Devolver referencia no constante
}

Map* GameLogic::get_map() const {
    return map;
}
