#ifndef GAME_AREA_H
#define GAME_AREA_H

#include <gtk/gtk.h>
#include "GameLogic.h"

class GameArea {
public:

    static GtkWidget* create(GameLogic* game_logic);
    static GtkWidget* create_projectile_widget(Projectile& projectile); // Nueva función para crear el widget del proyectil
    static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
    static void draw_damage_icon(cairo_t *cr, int x, int y);
    static gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data);  // Agregar la declaración
    static gboolean on_motion_notify(GtkWidget *widget, GdkEventMotion *event, gpointer user_data);  // Agregar evento de movimiento
    static gboolean on_draw_projectile(GtkWidget *widget, cairo_t *cr, gpointer user_data); // Prototipo de la función de dibujo del proyectil
    static GtkWidget* get_game_area() { return game_area; }
    static GtkWidget* create_tank_widget(Tank& tank);
    static GtkWidget* timer_label;  // NUEVO: Label para mostrar el temporizador
    static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
private:
    static GameLogic* game_logic;  // Agregar referencia a GameLogic
    static GtkWidget* game_area;  // Almacenar el widget del área de juego

};

#endif // GAME_AREA_H
