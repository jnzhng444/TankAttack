#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Map.h"
#include "Tank.h"
#include <gtk/gtk.h>

class GameLogic;  // Forward declaration para evitar dependencia circular

class Projectile {
public:
    double x, y;  // Posición del proyectil
    double direction_x, direction_y; // Dirección del proyectil
    double speed; // Velocidad del proyectil
    int map_width, map_height; // Dimensiones del mapa
    GtkWidget* widget; // Widget para dibujar el proyectil
    bool active = true;
    int rebotes; // Rebotes permitidos
    GameLogic* game_logic;  // Puntero a la lógica del juego
    Tank* shooter;  // Tanque que disparó el proyectil

    // Constructor
    Projectile(double start_x, double start_y, double dir_x, double dir_y, double spd, int map_width, int map_height, GameLogic* logic, Tank* shooter);

    // Método para actualizar la posición del proyectil
    void update();
    void handle_collision();
};

#endif // PROJECTILE_H
