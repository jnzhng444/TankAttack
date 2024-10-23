#include "GameArea.h"
#include "Map.h"
#include <iostream>
#include <math.h>

GameLogic* GameArea::game_logic = nullptr;  // Inicializar el puntero estático
Tank* selected_tank = nullptr;  // Tanque seleccionado por el jugador
GtkWidget* GameArea::game_area = nullptr;  // Definir la variable estática
GtkWidget* GameArea::timer_label = nullptr;  // Definir la variable estática

// En GameArea.cpp
GtkWidget* GameArea::create(GameLogic* logic) {
    // Obtener las dimensiones del mapa desde la lógica del juego
    int map_width = logic->get_map()->get_width();
    int map_height = logic->get_map()->get_height();
    int cell_size = 25;  // Tamaño de cada celda en píxeles

    // Calcular el tamaño total del área de juego en píxeles
    int total_width = map_width * cell_size;
    int total_height = map_height * cell_size;

    // Crear el contenedor para el área de juego
    game_area = gtk_fixed_new();
    gtk_widget_set_size_request(game_area, total_width, total_height);  // Ajustar el tamaño del contenedor al tamaño del mapa

    if (!game_area) {
        std::cerr << "Error al crear el widget del área de juego." << std::endl;
        return nullptr;
    }

    game_logic = logic;

    // Crear el área de dibujo sobre el contenedor
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, total_width, total_height);  // Ajustar el tamaño del área de dibujo al tamaño del mapa
    gtk_fixed_put(GTK_FIXED(game_area), drawing_area, 0, 0);

    // **Crear el GtkLabel para el temporizador**
    timer_label = gtk_label_new("Tiempo restante: 05:00");  // Asegúrate de crear el label
    gtk_fixed_put(GTK_FIXED(game_area), timer_label, total_width - -10, 5);  // Posicionarlo en la interfaz

    // Conectar señales de eventos del mouse a drawing_area
    g_signal_connect(drawing_area, "draw", G_CALLBACK(GameArea::on_draw), NULL);
    g_signal_connect(drawing_area, "button-press-event", G_CALLBACK(GameArea::on_button_press), game_logic);
    g_signal_connect(drawing_area, "motion-notify-event", G_CALLBACK(GameArea::on_motion_notify), game_logic);
    gtk_widget_add_events(drawing_area, GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK);

    return game_area;
}


gboolean GameArea::on_motion_notify(GtkWidget *widget, GdkEventMotion *event, gpointer user_data) {
    GameLogic* game_logic = static_cast<GameLogic*>(user_data);

    if (selected_tank != nullptr) {  // Solo dibujamos la línea si hay un tanque seleccionado
        int mouse_x = event->x / 25;  // Convertir la posición del mouse en coordenadas de celda
        int mouse_y = event->y / 25;  // Convertir la posición del mouse en coordenadas de celda

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
        if (!tank.is_active) continue; // Ignorar tanques inactivos

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

        // Dibujar la barra de salud del tanque (solo si está activo)
        double health_ratio = static_cast<double>(tank.health) / tank.max_health; // Proporción de salud
        int health_bar_length = static_cast<int>(health_ratio * (cell_width - 4)); // Longitud de la barra de salud

        // Posición de la barra de salud
        int bar_x = tank.y * cell_width + 2; // Un poco dentro del contorno
        int bar_y = tank.x * cell_height + cell_height; // Justo arriba del fondo del tanque

        // Dibujar fondo de la barra de salud
        cairo_set_source_rgb(cr, 0.5, 0.5, 0.5); // Color gris para el fondo
        cairo_rectangle(cr, bar_x, bar_y, cell_width - 4, 4); // Bar rectángulo de fondo
        cairo_fill(cr);

        // Dibujar la barra de salud
        cairo_set_source_rgb(cr, 0, 1, 0); // Color verde para la barra de salud
        cairo_rectangle(cr, bar_x, bar_y, health_bar_length, 4); // Bar rectángulo de salud
        cairo_fill(cr);

        // **Dibujar el símbolo de daño (solo si el tanque está activo)**
        if (tank.is_active) {
            int icon_x = tank.y * 25; // Coordenada X para el símbolo de daño
            int icon_y = tank.x * 25 - 10; // Coordenada Y para el símbolo de daño

            draw_damage_icon(cr, icon_x, icon_y); // Dibuja la "X" o símbolo de daño

            // Mostrar el valor numérico del daño
            char damage_str[32];
            snprintf(damage_str, sizeof(damage_str), "%d", tank.total_damage_taken);
            cairo_set_source_rgb(cr, 1, 0, 0);  // Color rojo para el texto
            cairo_move_to(cr, tank.y * 25 + 12, tank.x * 25 - 5);  // Coloca el texto cerca del ícono
            cairo_show_text(cr, damage_str);
        }
    }

    // Dibujar la línea de dirección si hay un tanque seleccionado
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



void GameArea::draw_damage_icon(cairo_t *cr, int x, int y) {
    // Color rojo
    cairo_set_source_rgb(cr, 1, 0, 0); // Rojo

    // Dibujar la primera línea de la "X"
    cairo_move_to(cr, x - 5, y - 5); // Punto inicial de la línea
    cairo_line_to(cr, x + 5, y + 5); // Punto final de la línea
    cairo_stroke(cr);                // Traza la línea

    // Dibujar la segunda línea de la "X"
    cairo_move_to(cr, x + 5, y - 5); // Punto inicial de la línea
    cairo_line_to(cr, x - 5, y + 5); // Punto final de la línea
    cairo_stroke(cr);                // Traza la línea
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
        if (!selected_tank) {
            std::cout << "No hay tanque seleccionado para disparar." << std::endl;
            return FALSE;  // No hacer nada si no hay tanque seleccionado
        }

        // Convertir las coordenadas del clic en términos de celdas del mapa
        int clicked_x = static_cast<int>(event->x / 25);
        int clicked_y = static_cast<int>(event->y / 25);

        // Establecer el objetivo de disparo con las coordenadas del clic
        game_logic->shoot(*selected_tank, clicked_x, clicked_y);  // Disparar hacia el objetivo

        std::cout << "Tanque del jugador " << selected_tank->player << " disparando desde ("
                  << selected_tank->x << ", " << selected_tank->y << ") hacia ("
                  << clicked_x << ", " << clicked_y << ")" << std::endl;

        // Cambiar turno inmediatamente después del disparo
        selected_tank = nullptr;  // Reiniciar la selección del tanque después del disparo

        return TRUE;  // Fin de la función al disparar
    }

    return FALSE;  // Ninguna acción se realizó
}

GtkWidget* GameArea::create_projectile_widget(Projectile& projectile) {
    GtkWidget *projectile_widget = gtk_drawing_area_new();
    gtk_widget_set_size_request(projectile_widget, 10, 10);  // Tamaño del proyectil

    // Conectar la señal de dibujo solo si se crea correctamente el widget
    if (GTK_IS_WIDGET(projectile_widget)) {
        g_signal_connect(projectile_widget, "draw", G_CALLBACK(on_draw_projectile), &projectile);
    } else {
        std::cerr << "Error: No se pudo crear el widget del proyectil." << std::endl;
    }

    return projectile_widget;
}

gboolean GameArea::on_draw_projectile(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    Projectile* projectile = static_cast<Projectile*>(user_data);

    // Dibujar el proyectil como un círculo rojo
    cairo_set_source_rgb(cr, 1, 0, 0); // Color rojo para el proyectil
    cairo_arc(cr, 5, 5, 5, 0, 2 * M_PI); // Dibujar el proyectil en el centro del widget (10x10 px)
    cairo_fill(cr);

    return FALSE;
}
