#ifndef GAME_AREA_H
#define GAME_AREA_H

#include <gtk/gtk.h>
#include "GameLogic.h"

class GameArea {
public:
    static GtkWidget* create(GameLogic* game_logic);
    static GtkWidget* create_projectile_widget(Projectile& projectile); // Nueva función para crear el widget del proyectil
    static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
    static gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data);  // Agregar la declaración
    static gboolean on_motion_notify(GtkWidget *widget, GdkEventMotion *event, gpointer user_data);  // Agregar evento de movimiento
    static gboolean on_draw_projectile(GtkWidget *widget, cairo_t *cr, gpointer user_data); // Prototipo de la función de dibujo del proyectil

private:
    static GameLogic* game_logic;  // Agregar referencia a GameLogic
};

#endif // GAME_AREA_H
