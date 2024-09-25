#include "GameLogic.h"

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

            // Definir color del tanque basado en el jugador
            if (player == 1) {
                // Jugador 1: un tanque azul y uno rojo
                tank.color = (i == 0) ? "blue" : "red";
            } else {
                // Jugador 2: un tanque celeste y uno amarillo
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



Map* GameLogic::get_map() const {  // Implementación del método get_map
    return map;
}

void GameLogic::move_tank(int tank_id, int x, int y) {
    // Lógica para mover un tanque
}

void GameLogic::shoot(int tank_id, int target_x, int target_y) {
    // Lógica para disparar desde un tanque
}

const std::vector<Tank>& GameLogic::get_tanks() const {
    return tanks;
}
