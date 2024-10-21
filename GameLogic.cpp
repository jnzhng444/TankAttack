#include "GameLogic.h"

#include <cmath>

#include "Pathfinding.h"  // Incluir el archivo de pathfinding
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "GameArea.h"

GameLogic::GameLogic(int num_tanks_per_player, Map* map)
    : num_tanks_per_player(num_tanks_per_player), map(map), current_player(1), game_time_left(300) {
    std::srand(std::time(0));  // Inicializar el generador aleatorio
}

void GameLogic::end_turn() {
    current_player = (current_player == 1) ? 2 : 1;  // Alternar entre jugador 1 y 2
    std::cout << "Es el turno del jugador " << current_player << std::endl;
}

void GameLogic::start_game_timer() {
    g_timeout_add_seconds(1, GameLogic::update_timer, this);  // Iniciar temporizador
}

gboolean GameLogic::update_timer(gpointer user_data) {
    GameLogic* logic = static_cast<GameLogic*>(user_data);

    // Disminuir el tiempo restante
    logic->game_time_left--;

    // Convertir el tiempo restante a formato mm:ss
    int minutes = logic->game_time_left / 60;
    int seconds = logic->game_time_left % 60;

    // Aumentar el tamaño de time_str para evitar truncamientos
    char time_str[32];  // Ahora tiene espacio suficiente para el formato
    snprintf(time_str, sizeof(time_str), "Tiempo restante: %02d:%02d", minutes, seconds);
    gtk_label_set_text(GTK_LABEL(logic->time_label), time_str);

    // Verificar si el tiempo se ha agotado
    if (logic->game_time_left <= 0) {
        logic->end_game();
        return FALSE;  // Detener el temporizador
    }

    return TRUE;  // Mantener el temporizador activo
}


void GameLogic::end_game() {
    int player1_tanks = 0;
    int player2_tanks = 0;

    // Contar los tanques de cada jugador
    for (const Tank& tank : tanks) {
        if (tank.player == 1) {
            player1_tanks++;
        } else if (tank.player == 2) {
            player2_tanks++;
        }
    }

    if (player1_tanks > player2_tanks) {
        std::cout << "¡El jugador 1 gana!" << std::endl;
    } else if (player2_tanks > player1_tanks) {
        std::cout << "¡El jugador 2 gana!" << std::endl;
    } else {
        std::cout << "¡Empate!" << std::endl;
    }
}

void GameLogic::generate_tanks() {
    for (int player = 1; player <= 2; ++player) {
        for (int i = 0; i < 2; ++i) {  // 2 tanques de cada color
            for (int j = 0; j < 2; ++j) {  // 2 tanques de cada jugador
                Tank tank;
                tank.id = player * 10 + i * 2 + j;  // ID único para cada tanque
                tank.player = player;
                tank.game_logic = this;  // Asignar puntero a game_logic
                tank.widget = nullptr;  // Asignar widget más adelante cuando se pase el área de juego

                // Definir color del tanque basado en el jugador
                if (player == 1) {
                    tank.color = (j == 0) ? "blue" : "red";  // Colores para el jugador 1
                } else {
                    tank.color = (j == 0) ? "lightblue" : "yellow";  // Colores para el jugador 2
                }

                // Encontrar una posición accesible para el tanque
                int x, y;
                do {
                    x = std::rand() % map->get_width();  // Usar get_width()
                    y = std::rand() % map->get_height();  // Usar get_height()
                } while (!map->is_accessible(x, y) || map->has_obstacle(x, y));

                tank.x = x;
                tank.y = y;
                tanks.push_back(tank);  // Agregar el tanque al vector
            }
        }
    }
}


void GameLogic::move_tank(int tank_id, int x, int y) {
    for (Tank& tank : tanks) {
        if (tank.id == tank_id) {
            // Verificar si la nueva posición es válida (sin obstáculos)
            if (x >= 0 && x < map->get_width() && y >= 0 && y < map->get_height() && !map->has_obstacle(x, y)) {
                tank.x = x;
                tank.y = y;
                std::cout << "Tanque " << tank_id << " movido a (" << x << ", " << y << ")" << std::endl;

                // Verificar que el widget es válido antes de redibujar
                if (GTK_IS_WIDGET(tank.widget)) {
                    gtk_widget_queue_draw(tank.widget);  // Redibujar el área de juego
                }
            } else {
                std::cout << "Movimiento inválido." << std::endl;
            }
            break;
        }
    }
}

void GameLogic::calculate_route(Tank& tank, int target_x, int target_y) {
    // Borrar la ruta anterior
    current_route.clear();

    int prob = std::rand() % 100;

    if (tank.color == "blue" || tank.color == "lightblue") {
        if (prob < 50) {
            std::vector<int> path = Pathfinding::bfs(map, tank.x, tank.y, target_x, target_y);
            if (!path.empty()) {
                tank.route = path;
                current_route = path;  // Guardar la ruta actual
                g_timeout_add(100, move_tank_step_by_step, &tank);
            }
        } else {
            random_movement_with_los(tank, target_x, target_y);
        }
    } else if (tank.color == "red" || tank.color == "yellow") {
        if (prob < 80) {
            std::vector<int> path = Pathfinding::dijkstra(map, tank.x, tank.y, target_x, target_y);
            if (!path.empty()) {
                tank.route = path;
                current_route = path;  // Guardar la ruta actual
                g_timeout_add(100, move_tank_step_by_step, &tank);
            }
        } else {
            random_movement_with_los(tank, target_x, target_y);
        }
    }
}

gboolean GameLogic::move_tank_step_by_step(gpointer data) {
    Tank* tank = static_cast<Tank*>(data);

    if (!tank->route.empty()) {
        int next_pos = tank->route.front();
        tank->route.erase(tank->route.begin());  // Eliminar el primer punto de la ruta
        int new_x = next_pos / tank->game_logic->map->get_width();
        int new_y = next_pos % tank->game_logic->map->get_width();

        // Mover el tanque a la nueva posición
        tank->game_logic->move_tank(tank->id, new_x, new_y);

        // Forzar el redibujado solo si el widget es válido
        if (GTK_IS_WIDGET(tank->widget)) {
            gtk_widget_queue_draw(tank->widget);  // Redibujar el área de dibujo
        } else {
            std::cout << "Widget no válido para redibujar." << std::endl;
        }

        return TRUE;  // Mantener el temporizador activo hasta que la ruta esté vacía
    }

    return FALSE;  // Detener el temporizador cuando no haya más movimientos
}

void GameLogic::random_movement_with_los(Tank& tank, int target_x, int target_y) {
    int new_x, new_y;
    bool moved = false;
    const int radius = 2;  // Definir el radio para el movimiento aleatorio

    // Primer intento: Mover en línea recta hacia el objetivo (sin romper línea de vista)
    if (target_x > tank.x) {
        new_x = tank.x + 1;
    } else if (target_x < tank.x) {
        new_x = tank.x - 1;
    } else {
        new_x = tank.x;
    }

    if (target_y > tank.y) {
        new_y = tank.y + 1;
    } else if (target_y < tank.y) {
        new_y = tank.y - 1;
    } else {
        new_y = tank.y;
    }

    // Verificar si la nueva posición está dentro del mapa y no tiene obstáculos
    if (new_x >= 0 && new_x < map->get_width() && new_y >= 0 && new_y < map->get_height() && !map->has_obstacle(new_x, new_y)) {
        // Mover el tanque a la nueva posición
        move_tank(tank.id, new_x, new_y);
        moved = true;
    } else {
        // Si no se pudo mover en línea recta, hacer un intento de movimiento aleatorio dentro del radio
        int rand_x = tank.x + (std::rand() % (2 * radius + 1)) - radius;
        int rand_y = tank.y + (std::rand() % (2 * radius + 1)) - radius;

        // Verificar que la posición aleatoria es válida y no tiene obstáculos
        if (rand_x >= 0 && rand_x < map->get_width() && rand_y >= 0 && rand_y < map->get_height() && !map->has_obstacle(rand_x, rand_y)) {
            // Mover el tanque a la posición aleatoria
            move_tank(tank.id, rand_x, rand_y);
            moved = true;
        }

        // Segundo intento: Si no se pudo mover aleatoriamente, avanzar hasta donde sea posible
        if (!moved) {
            if (target_x > tank.x && tank.x + 1 < map->get_width() && !map->has_obstacle(tank.x + 1, tank.y)) {
                move_tank(tank.id, tank.x + 1, tank.y);
            } else if (target_x < tank.x && tank.x - 1 >= 0 && !map->has_obstacle(tank.x - 1, tank.y)) {
                move_tank(tank.id, tank.x - 1, tank.y);
            } else if (target_y > tank.y && tank.y + 1 < map->get_height() && !map->has_obstacle(tank.x, tank.y + 1)) {
                move_tank(tank.id, tank.x, tank.y + 1);
            } else if (target_y < tank.y && tank.y - 1 >= 0 && !map->has_obstacle(tank.x, tank.y - 1)) {
                move_tank(tank.id, tank.x, tank.y - 1);
            }
        }
    }

    // Forzar redibujado del área del juego si el widget del tanque es válido
    if (GTK_IS_WIDGET(tank.widget)) {
        gtk_widget_queue_draw(tank.widget);
    }
}

// En GameLogic.cpp, dentro de la función `shoot` y `update`

// GameLogic.cpp

void GameLogic::shoot(Tank& tank, int aim_target_x, int aim_target_y) {
    // Convertir las coordenadas de clic (en celdas) a píxeles
    double tank_center_x = tank.y * 25 + 12.5;  // Centro del tanque en píxeles
    double tank_center_y = tank.x * 25 + 12.5;  // Centro del tanque en píxeles

    double target_x = aim_target_x * 25 + 12.5;  // Objetivo en píxeles
    double target_y = aim_target_y * 25 + 12.5;  // Objetivo en píxeles

    // Calcular la dirección desde el tanque hacia el objetivo en términos de píxeles
    double dx = target_x - tank_center_x;
    double dy = target_y - tank_center_y;

    double length = sqrt(dx * dx + dy * dy);

    // Normalizar el vector de dirección
    if (length > 0) {
        dx /= length;
        dy /= length;
    }

    // Crear el proyectil en la posición actual del tanque y pasar `this` como GameLogic*
    Projectile projectile(tank_center_x, tank_center_y, dx, dy, 18.0, map->get_width() * 25, map->get_height() * 25, this);
    projectile.active = true;
    projectiles.emplace_back(projectile);

    // Crear el widget del proyectil y agregarlo al área de juego
    GtkWidget* projectile_widget = GameArea::create_projectile_widget(projectiles.back());
    projectiles.back().widget = projectile_widget;

    GtkWidget* game_area = GameArea::get_game_area();
    if (GTK_IS_FIXED(game_area)) {
        gtk_fixed_put(GTK_FIXED(game_area), projectile_widget, tank_center_x, tank_center_y);  // Posición inicial en píxeles
        gtk_widget_show_all(game_area);

        // Añadir un temporizador para mover el proyectil
        g_timeout_add(50, [](gpointer data) -> gboolean {
            Projectile* proj = static_cast<Projectile*>(data);
            proj->update();

            // Si el proyectil ya no está activo (fuera del mapa o colisionó)
            if (!proj->active) {
                // Eliminar el widget del proyectil de manera segura
                if (GTK_IS_WIDGET(proj->widget)) {
                    gtk_widget_destroy(proj->widget);
                    proj->widget = nullptr;
                }
                return FALSE;  // Detener el temporizador
            }

            // Mover el proyectil si sigue activo
            if (GTK_IS_WIDGET(proj->widget)) {
                gtk_fixed_move(GTK_FIXED(GameArea::get_game_area()), proj->widget, proj->x, proj->y);  // Mover en píxeles
                gtk_widget_queue_draw(proj->widget);
            }

            return TRUE;  // Continuar el temporizador mientras el proyectil esté activo
        }, &projectiles.back());
    } else {
        std::cerr << "Error: game_area no es un contenedor GtkFixed válido." << std::endl;
    }

    // Cambiar el turno inmediatamente después de disparar
    end_turn();
}



void GameLogic::update() {
    update_projectiles();

    // Redibujar el área de juego
    for (Tank& tank : tanks) {
        if (GTK_IS_WIDGET(tank.widget)) {
            gtk_widget_queue_draw(tank.widget);
        }
    }

    // Redibujar el área de juego
    gtk_widget_queue_draw(GameArea::get_game_area());
}

void GameLogic::update_projectiles() {
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        it->update(); // Actualiza la posición del proyectil

        // Verificar si el proyectil ha salido del mapa o está inactivo
        if (!it->active || it->x < 0 || it->x >= map->get_width() || it->y < 0 || it->y >= map->get_height()) {
            // Comprobar si el widget del proyectil es válido antes de destruirlo
            if (it->widget && GTK_IS_WIDGET(it->widget)) {
                gtk_widget_destroy(it->widget); // Destruir el widget del proyectil de manera segura
                it->widget = nullptr; // Marcar el widget como nulo para evitar accesos futuros
            }
            it = projectiles.erase(it); // Eliminar el proyectil de la lista y avanzar al siguiente
        } else {
            // Comprobar si el widget es válido antes de redibujarlo o moverlo
            if (it->widget && GTK_IS_WIDGET(it->widget)) {
                gtk_fixed_move(GTK_FIXED(GameArea::get_game_area()), it->widget, it->y * 25, it->x * 25);
                gtk_widget_queue_draw(it->widget);
            }
            ++it; // Avanzar al siguiente proyectil
        }
    }
}






std::vector<Tank>& GameLogic::get_tanks() {
    return tanks;  // Devolver referencia no constante
}

Map* GameLogic::get_map() const {
    return map;
}
