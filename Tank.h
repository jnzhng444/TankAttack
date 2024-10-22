#ifndef TANK_H
#define TANK_H

#include <string>
#include <vector>
#include <gtk/gtk.h>  // Necesario si vas a usar GtkWidget para el widget del tanque

class GameLogic;  // Forward declaration para evitar dependencias circulares

class Tank {
public:
    // Atributos del tanque
    int id;
    int player;
    int x, y;
    std::string color;
    GtkWidget* widget;
    std::vector<int> route;
    GameLogic* game_logic;

    int max_health = 100;
    int health = 100;
    int total_damage_taken = 0;
    bool destroyed = false;  // Nuevo atributo
    bool is_active = true;

    // Constructor por defecto
    Tank() : id(-1), player(-1), x(0), y(0), widget(nullptr), game_logic(nullptr) {}

    // Método para verificar si el tanque está destruido
    bool is_destroyed() const {
        return health <= 0;  // Considerar el tanque destruido si su salud es 0 o menos
    }
};


#endif // TANK_H
