#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <gtk/gtk.h>

class GameWindow {
public:
    static void on_activate(GtkApplication *app, gpointer user_data);
};

#endif // GAME_WINDOW_H
