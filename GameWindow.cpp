#include "GameWindow.h"
#include "GameArea.h"
#include "GameLogic.h"

void GameWindow::on_activate(GtkApplication *app, gpointer user_data) {
    // Crear la ventana principal
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tank Attack");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);

    // Crear un contenedor vertical para el área de juego y la etiqueta del temporizador
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Crear la etiqueta para mostrar el tiempo restante
    GtkWidget *time_label = gtk_label_new("Tiempo restante: 5:00");  // Iniciar con 5 minutos
    gtk_box_pack_start(GTK_BOX(vbox), time_label, FALSE, FALSE, 0);

    // Crear el mapa y la lógica del juego
    Map* game_map = new Map(30, 30);
    game_map->generate_random_map();
    GameLogic* game_logic = new GameLogic(2, game_map);
    game_logic->generate_tanks();

    // Asignar la etiqueta de tiempo a game_logic para que pueda actualizarse
    game_logic->time_label = time_label;

    // Iniciar el temporizador de juego (5 minutos)
    game_logic->start_game_timer();

    // Crear el área de dibujo del juego
    GtkWidget *game_area = GameArea::create(game_logic);
    gtk_box_pack_start(GTK_BOX(vbox), game_area, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
}

