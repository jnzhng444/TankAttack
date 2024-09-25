#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <string>
#include <vector>
#include <glib.h>  // Incluir correctamente
#include <gtk/gtk.h>  // Incluir correctamente

#include "Map.h"

class GameLogic;  // Declaración adelantada de GameLogic
typedef struct _GtkWidget GtkWidget;  // Declaración adelantada de GtkWidget

struct Tank {
    int id;
    int x, y;  // Posición del tanque en el mapa
    int player;  // Jugador al que pertenece
    std::string color;  // Color del tanque
    std::vector<int> route;  // Ruta calculada
    GameLogic* game_logic;  // Puntero a la lógica del juego
    GtkWidget* widget;  // Puntero al área de dibujo para redibujar
};

class GameLogic {
public:
    GameLogic(int num_tanks_per_player, Map* map);
    void move_tank(int tank_id, int x, int y);
    void shoot(int tank_id, int target_x, int target_y);
    void generate_tanks();  // Generar los tanques
    std::vector<Tank>& get_tanks();  // Obtener los tanques
    Map* get_map() const;

    // Declaraciones faltantes
    void calculate_route(Tank& tank, int target_x, int target_y);  // Mover tanque según la ruta
    std::vector<int> bfs(int start_x, int start_y, int goal_x, int goal_y);  // BFS
    std::vector<int> dijkstra(int start_x, int start_y, int goal_x, int goal_y);  // Dijkstra
    void random_movement_with_los(Tank& tank, int target_x, int target_y);  // Movimiento aleatorio con línea de vista
    static gboolean move_tank_step_by_step(gpointer data);
private:
    Map* map;
    std::vector<Tank> tanks;
    int num_tanks_per_player;
    // Función para mover el tanque paso a paso

};

#endif  // GAME_LOGIC_H
