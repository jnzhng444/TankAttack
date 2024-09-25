#include "GameWindow.h"
#include "GameArea.h"
#include "GameLogic.h"

void GameWindow::on_activate(GtkApplication *app, gpointer user_data) {
    // Crear la ventana principal
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tank Attack");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);

    // Crear el mapa y la lógica del juego
    Map* game_map = new Map(30, 30);  // Aumenta el tamaño del mapa a 30x30
    game_map->generate_random_map();
    GameLogic* game_logic = new GameLogic(2, game_map);  // 2 tanques por jugador
    game_logic->generate_tanks();

    // Crear el área de dibujo del juego
    GtkWidget *game_area = GameArea::create(game_logic);
    gtk_container_add(GTK_CONTAINER(window), game_area);

    gtk_widget_show_all(window);
}
