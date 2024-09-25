#include "GameArea.h"
#include "Map.h"
#include <iostream>

GameLogic* GameArea::game_logic = nullptr;  // Inicializar el puntero estático

GtkWidget* GameArea::create(GameLogic* logic) {
    GtkWidget *area = gtk_drawing_area_new();
    gtk_widget_set_size_request(area, 800, 600);

    // Almacenar el puntero a GameLogic
    game_logic = logic;

    // Conectar señales de dibujo y eventos del mouse
    g_signal_connect(area, "draw", G_CALLBACK(GameArea::on_draw), NULL);

    return area;
}

gboolean GameArea::on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    // Definir el tamaño de cada celda
    int cell_width = 25;  // Ajusta el ancho de la celda a un valor menor
    int cell_height = 25; // Ajusta el alto de la celda a un valor menor

    // Definir el número de filas y columnas
    int rows = 30;  // Aumenta el número de filas
    int cols = 30;  // Aumenta el número de columnas

    // Dibujar el fondo del mapa (color de campo)
    cairo_set_source_rgb(cr, 0.9, 0.8, 0.6);  // Color arena
    cairo_paint(cr);

    // Dibujar cuadrícula
    cairo_set_source_rgb(cr, 0, 0, 0); // Líneas negras para la cuadrícula
    for (int i = 0; i <= rows; i++) {
        cairo_move_to(cr, 0, i * cell_height);
        cairo_line_to(cr, cols * cell_width, i * cell_height);
        cairo_stroke(cr);
    }

    for (int i = 0; i <= cols; i++) {
        cairo_move_to(cr, i * cell_width, 0);
        cairo_line_to(cr, i * cell_width, rows * cell_height);
        cairo_stroke(cr);
    }

    // Dibujar obstáculos (rojo)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (game_logic->get_map()->has_obstacle(i, j)) {
                cairo_set_source_rgb(cr, 0, 0, 0);  // Rojo para obstáculos
                cairo_rectangle(cr, j * cell_width, i * cell_height, cell_width, cell_height);
                cairo_fill(cr);
            }
        }
    }

    // Dibujar tanques
    const std::vector<Tank>& tanks = game_logic->get_tanks();
    for (const Tank& tank : tanks) {
        if (tank.color == "blue") {
            cairo_set_source_rgb(cr, 0, 0, 1);  // Azul
        } else if (tank.color == "red") {
            cairo_set_source_rgb(cr, 1, 0, 0);  // Rojo
        } else if (tank.color == "lightblue") {
            cairo_set_source_rgb(cr, 0.5, 0.8, 1);  // Celeste
        } else if (tank.color == "yellow") {
            cairo_set_source_rgb(cr, 1, 1, 0);  // Amarillo
        }

        // Dibujar el tanque como un rectángulo en su posición
        cairo_rectangle(cr, tank.y * cell_width, tank.x * cell_height, cell_width, cell_height);
        cairo_fill(cr);
    }

    return FALSE;
}

