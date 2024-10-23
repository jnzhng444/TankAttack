#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <vector>
#include "Map.h"
#include "Tank.h"  // Asegúrate de incluir la definición de Tank
#include "Pathfinding.h"
#include <glib.h>
#include <gtk/gtk.h>
#include "Projectile.h"

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

    // Otros métodos y variables...
    std::vector<Projectile> projectiles; // Lista de proyectiles

    void update_projectiles();
    void update();
    void on_click(int click_x, int click_y);
    // Método para disparar
    void shoot(Tank& tank, int aim_target_x, int aim_target_y);
    // Metodo para destruir el tanque
    void remove_tank(Tank& tank) ;
    std::vector<int> random_move(Tank& tank, int target_x, int target_y);  // Devuelve una lista de posiciones para el movimiento aleatorio
    void random_move_step_by_step(Tank& tank, int target_x, int target_y);  // Movimiento aleatorio controlado por temporizador
    // Declaraciones de las funciones nuevas
    bool check_line_of_sight(Tank& tank, int target_x, int target_y);   // Verifica la línea de vista
    std::pair<int, int> get_random_position(int x, int y, int radius);  // Genera una posición aleatoria
    void move_tank_towards(Tank& tank, int target_x, int target_y);     // Mueve el tanque hacia el objetivo
    void move_tank_towards_step_by_step(Tank& tank, int target_x, int target_y, int step_distance);  // Nueva función
    // Obtener el puntero al mapa
    Map* get_map() const;

    std::vector<int> current_route; // Ruta actual calculada
    void end_turn();               // Alternar turnos entre jugadores
    void start_game_timer();        // Iniciar el temporizador del juego
    static gboolean update_timer(gpointer user_data);  // Actualizar el temporizador
    void end_game();               // Termina el juego y determina el ganador
    int current_player;
    GtkWidget* time_label;    // Etiqueta para mostrar el tiempo restante en la interfaz
    int aim_target_x;  // Coordenada X del objetivo
    int aim_target_y;  // Coordenada Y del objetivo
    std::vector<Tank*> tanks_to_remove;      // Lista de tanques para eliminar
    std::vector<Projectile*> projectiles_to_remove;  // Lista de proyectiles para eliminar
    void mark_tank_for_removal(Tank* tank);      // Nuevo método para marcar tanques para eliminación
    void mark_projectile_for_removal(Projectile* projectile); // Nuevo método para marcar proyectiles
    void process_removals();    // Procesa las eliminaciones pendientes

private:
    int num_tanks_per_player;       // Número de tanques por jugador
    std::vector<Tank> tanks;        // Vector de tanques
    int game_time_left;            // Tiempo restante en segundos
    Map* map;                       // Puntero al mapa de juego
};

#endif // GAMELOGIC_H
