#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <string>
#include <vector>
#include "Map.h"

struct Tank {
    int id;
    int x, y;  // Posición del tanque en el mapa
    int player;  // Jugador al que pertenece
    std::string color;  // Color del tanque
};

class GameLogic {
public:
    GameLogic(int num_tanks_per_player, Map* map);
    void move_tank(int tank_id, int x, int y);
    void shoot(int tank_id, int target_x, int target_y);
    void generate_tanks();  // Generar los tanques
    const std::vector<Tank>& get_tanks() const;  // Obtener los tanques
    Map* get_map() const;  // Nuevo método para obtener el mapa

private:
    Map* map;
    std::vector<Tank> tanks;
    int num_tanks_per_player;
};

#endif  // GAME_LOGIC_H
