#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <vector>
#include "Map.h"
#include "Tank.h"  // Asegúrate de incluir la definición de Tank
#include "Pathfinding.h"
#include <glib.h>
#include <gtk/gtk.h>

class GameLogic {
public:
    // Constructor
    GameLogic(int num_tanks_per_player, Map* map);

    // Generar tanques
    void generate_tanks();

    // Mover un tanque a una nueva posición
    void move_tank(int tank_id, int x, int y);

    // Calcular la ruta para un tanque
    void calculate_route(Tank& tank, int target_x, int target_y);

    // Movimiento paso a paso de un tanque
    static gboolean move_tank_step_by_step(gpointer data);

    // Movimiento aleatorio con línea de vista
    void random_movement_with_los(Tank& tank, int target_x, int target_y);

    // Obtener la referencia a la lista de tanques
    std::vector<Tank>& get_tanks();

    // Obtener el puntero al mapa
    Map* get_map() const;

    std::vector<int> current_route; // Ruta actual calculada
    void end_turn();               // Alternar turnos entre jugadores
    void start_game_timer();        // Iniciar el temporizador del juego
    static gboolean update_timer(gpointer user_data);  // Actualizar el temporizador
    void end_game();               // Termina el juego y determina el ganador
    int current_player;
    GtkWidget* time_label;    // Etiqueta para mostrar el tiempo restante en la interfaz

private:
    int num_tanks_per_player;       // Número de tanques por jugador
    std::vector<Tank> tanks;        // Vector de tanques
    int game_time_left;            // Tiempo restante en segundos
    Map* map;                       // Puntero al mapa de juego
};

#endif // GAMELOGIC_H
