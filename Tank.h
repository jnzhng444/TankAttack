#ifndef TANK_H
#define TANK_H

#include <string>
#include <vector>
#include <gtk/gtk.h>  // Necesario si vas a usar GtkWidget para el widget del tanque

class GameLogic;  // Forward declaration para evitar dependencias circulares

class Tank {
public:
    // Atributos del tanque
    int id;                     // ID único para el tanque
    int player;                 // Jugador que controla el tanque
    int x, y;                   // Posición del tanque en el mapa
    std::string color;          // Color del tanque
    GtkWidget* widget;          // Widget para dibujar el tanque
    std::vector<int> route;     // Ruta calculada para el tanque
    GameLogic* game_logic;      // Puntero a la lógica del juego para referenciarla si es necesario

    // Constructor por defecto
    Tank() : id(-1), player(-1), x(0), y(0), widget(nullptr), game_logic(nullptr) {}

    int max_health = 100;
    int health = 100;  // Salud actual
    int total_damage_taken = 0;  // Daño total recibido
};

#endif // TANK_H
