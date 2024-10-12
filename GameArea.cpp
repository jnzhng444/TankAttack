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
    g_signal_connect(area, "button-press-event", G_CALLBACK(GameArea::on_button_press), game_logic);
    g_signal_connect(area, "motion-notify-event", G_CALLBACK(GameArea::on_motion_notify), game_logic);  // Conectar el evento de movimiento
    gtk_widget_add_events(area, GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK);

    return area;
}

gboolean GameArea::on_motion_notify(GtkWidget *widget, GdkEventMotion *event, gpointer user_data) {
    GameLogic* game_logic = static_cast<GameLogic*>(user_data);

    if (selected_tank != nullptr) {  // Solo dibujamos la línea si hay un tanque seleccionado
        int mouse_x = event->x / 25;
        int mouse_y = event->y / 25;

        // Guardar las coordenadas del objetivo (el punto al que se apunta)
        game_logic->aim_target_x = mouse_x;
        game_logic->aim_target_y = mouse_y;

        // Redibujar el área de juego para mostrar la línea de dirección
        gtk_widget_queue_draw(widget);
    }

    return TRUE;
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
    // Dibujar proyectiles
    cairo_set_source_rgb(cr, 1, 0, 0); // Color rojo para los proyectiles
    for (const Projectile& projectile : game_logic->projectiles) {
        cairo_arc(cr, projectile.y * cell_width + cell_width / 2, projectile.x * cell_height + cell_height / 2, 3, 0, 2 * M_PI);
        cairo_fill(cr);
    }

    // Dibujar la línea de dirección si hay un tanque seleccionado y el cursor no está encima del tanque
    if (selected_tank != nullptr) {
        // Coordenadas del tanque
        int tank_center_x = selected_tank->y * cell_width + cell_width / 2;
        int tank_center_y = selected_tank->x * cell_height + cell_height / 2;

        // Coordenadas del objetivo (posición del ratón)
        int aim_x = game_logic->aim_target_x * cell_width + cell_width / 2;
        int aim_y = game_logic->aim_target_y * cell_height + cell_height / 2;

        // Calcular si el cursor está encima del tanque
        if (!(aim_x >= tank_center_x - cell_width / 2 && aim_x <= tank_center_x + cell_width / 2 &&
              aim_y >= tank_center_y - cell_height / 2 && aim_y <= tank_center_y + cell_height / 2)) {

            // Calcular la dirección entre el tanque y el cursor
            double dx = aim_x - tank_center_x;
            double dy = aim_y - tank_center_y;
            double distance = sqrt(dx * dx + dy * dy);

            // Limitar la longitud máxima de la línea
            double max_length = 15.0;  // Ajusta este valor según lo que necesites
            double line_length = std::min(max_length, distance);  // Escoger la longitud más pequeña entre la distancia y la longitud fija

            // Calcular las coordenadas finales de la línea con longitud limitada
            double end_x = tank_center_x + (dx / distance) * line_length;
            double end_y = tank_center_y + (dy / distance) * line_length;

            // Dibujar la línea desde el tanque hacia la dirección deseada (con longitud limitada)
            cairo_set_source_rgb(cr, 1, 1, 1);  // Color blanco para la línea
            cairo_set_line_width(cr, 2);  // Línea delgada
            cairo_move_to(cr, tank_center_x, tank_center_y);
            cairo_line_to(cr, end_x, end_y);
            cairo_stroke(cr);
              }
    }

    return FALSE;

}


gboolean GameArea::on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
    int clicked_x = event->y / 25;  // Dividir la posición del clic por el tamaño de las celdas
    int clicked_y = event->x / 25;

    GameLogic* game_logic = static_cast<GameLogic*>(user_data);  // Obtener el puntero a la lógica del juego

    if (event->button == 1) {  // Clic izquierdo para seleccionar o mover
        if (selected_tank == nullptr) {  // Si no hay tanque seleccionado, seleccionar uno
            for (Tank& tank : game_logic->get_tanks()) {
                if (tank.x == clicked_x && tank.y == clicked_y && tank.player == game_logic->current_player) {
                    selected_tank = &tank;
                    selected_tank->widget = widget;  // Asociar el área de dibujo al tanque seleccionado
                    std::cout << "Tanque del jugador " << tank.player << " seleccionado en ("
                              << clicked_x << ", " << clicked_y << ")" << std::endl;
                    return TRUE; // Fin de la función al seleccionar el tanque
                }
            }
        } else {  // Si ya hay un tanque seleccionado, moverlo
            if (selected_tank->player == game_logic->current_player) {
                selected_tank->widget = widget;  // Asegurarse de que el widget esté asociado antes de mover
                game_logic->calculate_route(*selected_tank, clicked_x, clicked_y);  // Calcular la ruta hacia el destino
                g_timeout_add(100, GameLogic::move_tank_step_by_step, selected_tank);  // Mover tanque paso a paso
                game_logic->end_turn();  // Cambiar turno
                selected_tank = nullptr;  // Reiniciar la selección del tanque después de mover
                return TRUE; // Fin de la función al mover el tanque
            }
        }
    } else if (event->button == 3 && selected_tank != nullptr && selected_tank->player == game_logic->current_player) {  // Clic derecho para disparar
        game_logic->aim_target_x = clicked_x; // Establecer el objetivo de disparo
        game_logic->aim_target_y = clicked_y; // Establecer el objetivo de disparo
        game_logic->shoot(*selected_tank); // Disparar hacia el objetivo
        std::cout << "Tanque del jugador " << selected_tank->player << " disparando desde ("
                  << selected_tank->x << ", " << selected_tank->y << ") hacia ("
                  << clicked_x << ", " << clicked_y << ")" << std::endl;
        return TRUE; // Fin de la función al disparar
    }

    return FALSE; // Ninguna acción se realizó
}
