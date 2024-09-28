#include "GameArea.h"
#include "Map.h"
#include <iostream>
#include <math.h>

GameLogic* GameArea::game_logic = nullptr;  // Inicializar el puntero estático
Tank* selected_tank = nullptr;  // Tanque seleccionado por el jugador

GtkWidget* GameArea::create(GameLogic* logic) {
    GtkWidget *area = gtk_drawing_area_new();
    gtk_widget_set_size_request(area, 800, 600);

    // Almacenar el puntero a GameLogic
    game_logic = logic;

    // Conectar señales de dibujo y eventos del mouse
    g_signal_connect(area, "draw", G_CALLBACK(GameArea::on_draw), NULL);
    g_signal_connect(area, "button-press-event", G_CALLBACK(GameArea::on_button_press), NULL);
    gtk_widget_add_events(area, GDK_BUTTON_PRESS_MASK);  // Permitir eventos de clic

    return area;
}

gboolean GameArea::on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    // Definir el tamaño de cada celda
    int cell_width = 25;  // Ajusta el ancho de la celda
    int cell_height = 25; // Ajusta el alto de la celda

    int rows = 30;
    int cols = 30;

    // Establecer el color de fondo
    //cairo_set_source_rgb(cr, 0.9, 0.9, 0.9);  // Color gris claro, puedes cambiarlo por cualquier color
    //cairo_rectangle(cr, 0, 0, cols * cell_width, rows * cell_height);  // Dibuja el fondo para todo el área
    //cairo_fill(cr);

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

    // Dibujar obstáculos (negro)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (game_logic->get_map()->has_obstacle(i, j)) {
                cairo_set_source_rgb(cr, 0, 0, 0);  // Negro para obstáculos
                cairo_rectangle(cr, j * cell_width, i * cell_height, cell_width, cell_height);
                cairo_fill(cr);
            }
        }
    }

    // Dibujar la ruta actual
    const std::vector<int>& route = game_logic->current_route;
    if (!route.empty()) {
        cairo_set_source_rgb(cr, 0, 1, 0);  // Color verde para la ruta
        for (size_t i = 1; i < route.size(); ++i) {
            int prev_x = route[i - 1] / cols;
            int prev_y = route[i - 1] % cols;
            int curr_x = route[i] / cols;
            int curr_y = route[i] % cols;

            cairo_move_to(cr, prev_y * cell_width + cell_width / 2, prev_x * cell_height + cell_height / 2);
            cairo_line_to(cr, curr_y * cell_width + cell_width / 2, curr_x * cell_height + cell_height / 2);
            cairo_stroke(cr);
        }
    }

    // Dibujar tanques más detallados
    const std::vector<Tank>& tanks = game_logic->get_tanks();
    for (const Tank& tank : tanks) {
        // Resaltar el color del tanque
        if (tank.color == "blue") {
            cairo_set_source_rgb(cr, 0, 0, 1);  // Azul
        } else if (tank.color == "red") {
            cairo_set_source_rgb(cr, 1, 0, 0);  // Rojo
        } else if (tank.color == "lightblue") {
            cairo_set_source_rgb(cr, 0.5, 0.8, 1);  // Celeste
        } else if (tank.color == "yellow") {
            cairo_set_source_rgb(cr, 1, 1, 0);  // Amarillo
        }

        // Dibujar el contorno del tanque
        cairo_set_line_width(cr, 2);  // Contorno más grueso
        cairo_set_source_rgb(cr, 0, 0, 0);  // Negro para el contorno
        cairo_rectangle(cr, tank.y * cell_width + 2, tank.x * cell_height + 2, cell_width - 4, cell_height - 4);
        cairo_stroke(cr);

        // Dibujar el cuerpo del tanque dentro del contorno
        cairo_set_source_rgb(cr,
            tank.color == "blue" ? 0 : tank.color == "red" ? 1 : tank.color == "lightblue" ? 0.5 : 1,
            tank.color == "blue" ? 0 : tank.color == "red" ? 0 : tank.color == "lightblue" ? 0.8 : 1,
            tank.color == "blue" ? 1 : tank.color == "red" ? 0 : tank.color == "lightblue" ? 1 : 0);
        cairo_rectangle(cr, tank.y * cell_width + 4, tank.x * cell_height + 4, cell_width - 8, cell_height - 8);
        cairo_fill(cr);

        // Dibujar la torreta (círculo en el centro del tanque)
        cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);  // Color más oscuro para la torreta
        cairo_arc(cr, tank.y * cell_width + cell_width / 2, tank.x * cell_height + cell_height / 2, cell_width / 6, 0, 2 * M_PI);
        cairo_fill(cr);

        // Dibujar el cañón (una línea que sale de la torreta)
        cairo_set_source_rgb(cr, 0, 0, 0);  // Color negro para el cañón
        cairo_set_line_width(cr, 2);
        cairo_move_to(cr, tank.y * cell_width + cell_width / 2, tank.x * cell_height + cell_height / 2);
        cairo_line_to(cr, tank.y * cell_width + cell_width / 2, tank.x * cell_height + 2);  // Cañón hacia arriba
        cairo_stroke(cr);
    }

    return FALSE;
}




gboolean GameArea::on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
    int clicked_x = event->y / 25;  // Dividir la posición del clic por el tamaño de las celdas
    int clicked_y = event->x / 25;

    if (event->button == 1) {  // Clic derecho para seleccionar el tanque
        for (Tank& tank : game_logic->get_tanks()) {
            if (tank.x == clicked_x && tank.y == clicked_y) {
                selected_tank = &tank;
                selected_tank->widget = widget;  // Asociar el área de dibujo al tanque seleccionado
                std::cout << "Tanque seleccionado en (" << clicked_x << ", " << clicked_y << ")" << std::endl;
                return TRUE;
            }
        }
    } else if (event->button == 3 && selected_tank != nullptr) {  // Clic izquierdo para mover
        selected_tank->widget = widget;  // Asegurarse de que el widget esté asociado antes de mover
        game_logic->calculate_route(*selected_tank, clicked_x, clicked_y);  // Calcular la ruta hacia el destino
        g_timeout_add(100, GameLogic::move_tank_step_by_step, selected_tank);  // Mover tanque paso a paso
        return TRUE;
    }

    return FALSE;
}




