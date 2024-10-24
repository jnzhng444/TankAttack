#include "GameWindow.h"
#include "GameArea.h"
#include "GameLogic.h"

void GameWindow::on_activate(GtkApplication *app, gpointer user_data) {
    // Crear la ventana principal
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tank Attack");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);  // Permitir cambiar tamaño

    // Crear un contenedor horizontal para el área de juego y la barra de información
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_add(GTK_CONTAINER(window), hbox);

    // Crear el mapa y la lógica del juego
    Map* game_map = new Map(30, 30);
    game_map->generate_random_map();
    GameLogic* game_logic = new GameLogic(2, game_map);
    game_logic->generate_tanks();

    // Crear el área de dibujo del juego
    GtkWidget *game_area = GameArea::create(game_logic);
    gtk_box_pack_start(GTK_BOX(hbox), game_area, TRUE, TRUE, 0);

    // Crear un contenedor vertical para la barra de información
    GtkWidget *info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);  // Espaciado vertical
    gtk_box_pack_start(GTK_BOX(hbox), info_box, FALSE, FALSE, 0);

    // Etiqueta de Jugador 1
    GtkWidget *player1_label = gtk_label_new("Jugador 1:");
    gtk_box_pack_start(GTK_BOX(info_box), player1_label, FALSE, FALSE, 0);

    // Cantidad de tanques restantes
    GtkWidget *player1_tank_count = gtk_label_new("Cantidad de tanques restantes: ");
    gtk_box_pack_start(GTK_BOX(info_box), player1_tank_count, FALSE, FALSE, 0);

    GtkWidget *player1_tank_color= gtk_label_new("Colores de tanques usados: ");
    gtk_box_pack_start(GTK_BOX(info_box), player1_tank_color, FALSE, FALSE, 0);

    // Colores de los tanques del jugador 1
    GtkWidget *player1_color_red = gtk_label_new("Rojo");
    GdkColor red;
    gdk_color_parse("red", &red);
    gtk_widget_modify_fg(player1_color_red, GTK_STATE_NORMAL, &red);
    gtk_box_pack_start(GTK_BOX(info_box), player1_color_red, FALSE, FALSE, 0);

    GtkWidget *player1_color_blue = gtk_label_new("Azul");
    GdkColor blue;
    gdk_color_parse("blue", &blue);
    gtk_widget_modify_fg(player1_color_blue, GTK_STATE_NORMAL, &blue);
    gtk_box_pack_start(GTK_BOX(info_box), player1_color_blue, FALSE, FALSE, 0);

    // Espacio entre Jugador 1 y Jugador 2
    GtkWidget *separator1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(info_box), separator1, FALSE, FALSE, 5);

    // Etiqueta de Jugador 2
    GtkWidget *player2_label = gtk_label_new("Jugador 2:");
    gtk_box_pack_start(GTK_BOX(info_box), player2_label, FALSE, FALSE, 0);

    // Cantidad de tanques restantes
    GtkWidget *player2_tank_count = gtk_label_new("Cantidad de tanques restantes: ");
    gtk_box_pack_start(GTK_BOX(info_box), player2_tank_count, FALSE, FALSE, 0);

    GtkWidget *player2_tank_color= gtk_label_new("Colores de tanques usados: ");
    gtk_box_pack_start(GTK_BOX(info_box), player2_tank_color, FALSE, FALSE, 0);

    // Colores de los tanques del jugador 2
    GtkWidget *player2_color_yellow = gtk_label_new("Amarillo");
    GdkColor yellow;
    gdk_color_parse("yellow", &yellow);
    gtk_widget_modify_fg(player2_color_yellow, GTK_STATE_NORMAL, &yellow);
    gtk_box_pack_start(GTK_BOX(info_box), player2_color_yellow, FALSE, FALSE, 0);

    GtkWidget *player2_color_cyan = gtk_label_new("Celeste");
    GdkColor cyan;
    gdk_color_parse("cyan", &cyan);
    gtk_widget_modify_fg(player2_color_cyan, GTK_STATE_NORMAL, &cyan);
    gtk_box_pack_start(GTK_BOX(info_box), player2_color_cyan, FALSE, FALSE, 0);

    // Establecer la fuente (ajusta el nombre a la fuente que desees)
    PangoFontDescription *font_desc = pango_font_description_from_string("Press Start 2P 14");
    gtk_widget_modify_font(player1_label, font_desc);
    gtk_widget_modify_font(player1_tank_count, font_desc);
    gtk_widget_modify_font(player1_tank_color, font_desc);
    gtk_widget_modify_font(player1_color_red, font_desc);
    gtk_widget_modify_font(player1_color_blue, font_desc);
    gtk_widget_modify_font(player2_label, font_desc);
    gtk_widget_modify_font(player2_tank_count, font_desc);
    gtk_widget_modify_font(player2_tank_color, font_desc);
    gtk_widget_modify_font(player2_color_yellow, font_desc);
    gtk_widget_modify_font(player2_color_cyan, font_desc);

    // Liberar la descripción de la fuente
    pango_font_description_free(font_desc);

    // Mostrar todo
    gtk_widget_show_all(window);
}
