#ifndef GAME_AREA_H
#define GAME_AREA_H

#include <gtk/gtk.h>
#include "GameLogic.h"

class GameArea {
public:
    static GtkWidget* create(GameLogic* game_logic);
    static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data);
private:
    static GameLogic* game_logic;  // Agregar referencia a GameLogic
};

#endif // GAME_AREA_H
