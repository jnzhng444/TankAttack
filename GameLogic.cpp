#include "GameLogic.h"

#include <algorithm>
#include <cmath>

#include "Pathfinding.h"  // Incluir el archivo de pathfinding
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>

#include "GameArea.h"

GameLogic::GameLogic(int num_tanks_per_player, Map* map)
    : num_tanks_per_player(num_tanks_per_player), map(map), current_player(1), game_time_left(300) {
    std::srand(std::time(0));  // Inicializar el generador aleatorio

    start_game_timer();

}

void GameLogic::end_turn() {
    current_player = (current_player == 1) ? 2 : 1;  // Alternar entre jugador 1 y 2
    std::cout << "Es el turno del jugador " << current_player << std::endl;

    // Limpiar la traza de las balas al finalizar el turno
    projectile_trail.clear();

    // Redibujar el área de juego para reflejar los cambios
    gtk_widget_queue_draw(GameArea::get_game_area());
}


bool GameLogic::check_victory() {
    int player1_tanks = 0;
    int player2_tanks = 0;

    // Contar los tanques activos de cada jugador
    for (const Tank& tank : tanks) {
        if (tank.is_active) {
            if (tank.player == 1) {
                player1_tanks++;
            } else if (tank.player == 2) {
                player2_tanks++;
            }
        }
    }

    // Si un jugador no tiene tanques activos, el otro gana
    if (player1_tanks == 0) {
        show_victory_dialog(2);  // Jugador 2 gana
        return true;
    } else if (player2_tanks == 0) {
        show_victory_dialog(1);  // Jugador 1 gana
        return true;
    }

    // Si el tiempo se acaba, gana el jugador con más tanques
    if (game_time_left <= 0) {
        if (player1_tanks > player2_tanks) {
            show_victory_dialog(1);  // Jugador 1 gana por tener más tanques
        } else if (player2_tanks > player1_tanks) {
            show_victory_dialog(2);  // Jugador 2 gana por tener más tanques
        } else {
            show_victory_dialog(0);  // Empate
        }
        return true;
    }

    return false;  // No hay ganador todavía
}


void GameLogic::show_victory_dialog(int winning_player) {
    // Crear y mostrar un diálogo emergente con el jugador ganador
    GtkWidget* dialog = gtk_message_dialog_new(nullptr, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "¡El jugador %d ha ganado!", winning_player);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    // Detener el juego aquí si es necesario
}


void GameLogic::start_game_timer() {
    // Configurar un temporizador que se actualiza cada segundo
    g_timeout_add_seconds(1, GameLogic::update_timer, this);  // Iniciar temporizador
}

gboolean GameLogic::update_timer(gpointer user_data) {
    GameLogic* logic = static_cast<GameLogic*>(user_data);

    // Disminuir el tiempo restante
    logic->game_time_left--;

    // Convertir el tiempo restante a formato mm:ss
    int minutes = logic->game_time_left / 60;
    int seconds = logic->game_time_left % 60;

    // Asegurarse de que el timer_label es un GtkLabel antes de actualizarlo
    if (GTK_IS_LABEL(GameArea::timer_label)) {
        char time_str[32];  // Ahora tiene espacio suficiente para el formato
        snprintf(time_str, sizeof(time_str), "Tiempo restante: %02d:%02d", minutes, seconds);
        gtk_label_set_text(GTK_LABEL(GameArea::timer_label), time_str);
    } else {
        std::cerr << "timer_label no es un GtkLabel válido." << std::endl;
    }

    // Verificar si el tiempo se ha agotado
    if (logic->game_time_left <= 0) {
        // Si se agota el tiempo, verificar qué jugador tiene más tanques
        logic->check_victory();  // Verifica la condición de victoria basada en los tanques restantes
        return FALSE;  // Detener el temporizador
    }

    // Verificar la condición de victoria en cada ciclo del temporizador
    if (logic->check_victory()) {
        return FALSE;  // Si hay un ganador, detener el temporizador
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
    int player1_column = 1;  // Columna para el jugador 1
    int player2_column = map->get_width() - 2;  // Columna para el jugador 2
    int row_offset = 7;  // Separación entre tanques por filas

    int player1_row = 0;  // Primera fila para el jugador 1
    int player2_row = 0;  // Primera fila para el jugador 2

    for (int player = 1; player <= 2; ++player) {
        for (int i = 0; i < 2; ++i) {  // 2 colores por jugador
            for (int j = 0; j < 2; ++j) {  // 2 tanques por color
                Tank tank;
                tank.id = player * 10 + i * 2 + j;  // ID único para cada tanque
                tank.player = player;
                tank.game_logic = this;

                bool found_position = false;
                int attempts = 0;  // Contador de intentos

                // Intentar encontrar una fila sin obstáculos para el tanque
                while (!found_position && attempts < 10) {  // Limitar los intentos a 10
                    attempts++;

                    // Definir color del tanque basado en el jugador
                    if (player == 1) {
                        tank.color = (j == 0) ? "blue" : "red";  // Colores para el jugador 1
                        tank.x = player1_row;  // Asignar fila
                        tank.y = player1_column;  // Asignar columna

                        // Verificar si la posición tiene un obstáculo
                        if (!map->has_obstacle(tank.x, tank.y)) {
                            found_position = true;
                        } else {
                            // Intentar spawnear arriba o abajo del obstáculo
                            if (tank.x > 0 && !map->has_obstacle(tank.x - 1, tank.y)) {
                                tank.x -= 1;  // Mover una fila arriba
                                found_position = true;
                            } else if (tank.x < map->get_height() - 1 && !map->has_obstacle(tank.x + 1, tank.y)) {
                                tank.x += 1;  // Mover una fila abajo
                                found_position = true;
                            } else {
                                player1_row += 1;  // Mover una fila abajo si ambas posiciones están bloqueadas
                            }
                        }
                    } else {
                        tank.color = (j == 0) ? "lightblue" : "yellow";  // Colores para el jugador 2
                        tank.x = player2_row;  // Asignar fila
                        tank.y = player2_column;  // Asignar columna

                        // Verificar si la posición tiene un obstáculo
                        if (!map->has_obstacle(tank.x, tank.y)) {
                            found_position = true;
                        } else {
                            // Intentar spawnear arriba o abajo del obstáculo
                            if (tank.x > 0 && !map->has_obstacle(tank.x - 1, tank.y)) {
                                tank.x -= 1;  // Mover una fila arriba
                                found_position = true;
                            } else if (tank.x < map->get_height() - 1 && !map->has_obstacle(tank.x + 1, tank.y)) {
                                tank.x += 1;  // Mover una fila abajo
                                found_position = true;
                            } else {
                                player2_row += 1;  // Mover una fila abajo si ambas posiciones están bloqueadas
                            }
                        }
                    }
                }

                // Incrementar la fila para el siguiente tanque después de encontrar una posición válida
                if (player == 1) {
                    player1_row += row_offset;
                } else {
                    player2_row += row_offset;
                }

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
    current_route.clear();  // Limpiar la ruta actual

    int prob = std::rand() % 100;

    if (tank.color == "blue" || tank.color == "lightblue") {
        if (prob < 50) {
            std::vector<int> path = Pathfinding::bfs(map, tank.x, tank.y, target_x, target_y);
            if (!path.empty()) {
                tank.route = path;
                current_route = path;  // Guardar la ruta actual
                g_timeout_add(100, move_tank_step_by_step, &tank);  // Mover paso a paso
            }
        } else {
            random_move_step_by_step(tank, target_x, target_y);  // Movimiento aleatorio paso a paso con temporizador
        }
    } else if (tank.color == "red" || tank.color == "yellow") {
        if (prob < 80) {
            std::vector<int> path = Pathfinding::dijkstra(map, tank.x, tank.y, target_x, target_y);
            if (!path.empty()) {
                tank.route = path;
                current_route = path;  // Guardar la ruta actual
                g_timeout_add(100, move_tank_step_by_step, &tank);  // Mover paso a paso
            }
        } else {
            random_move_step_by_step(tank, target_x, target_y);  // Movimiento aleatorio paso a paso con temporizador
        }
    }
}

void GameLogic::random_move_step_by_step(Tank& tank, int target_x, int target_y) {
    static int attempt_count = 0;  // Contador de intentos

    // Limitar el número de intentos para evitar bucles
    const int max_attempts = 10;  // Define el número máximo de intentos permitidos

    if (attempt_count >= max_attempts) {
        std::cout << "Movimiento aleatorio detenido: se alcanzó el número máximo de intentos (" << max_attempts << ")" << std::endl;
        attempt_count = 0;  // Reiniciar el contador para el próximo movimiento
        return;  // Detener el movimiento
    }

    if (tank.x == target_y && tank.y == target_x) {
        std::cout << "Tanque ha llegado al destino: (" << target_x << ", " << target_y << ")" << std::endl;
        attempt_count = 0;  // Reiniciar el contador al alcanzar el destino
        return;
    }

    if (check_line_of_sight(tank, target_x, target_y)) {
        std::cout << "Línea de vista despejada hacia (" << target_x << ", " << target_y << ")" << std::endl;
        move_tank_towards_step_by_step(tank, target_x, target_y, 1);

        // Añadir a la ruta para dibujar la traza
        int current_pos = tank.x * map->get_width() + tank.y;
        current_route.push_back(current_pos);  // Guardar la posición actual
    } else {
        std::cout << "Línea de vista bloqueada, seleccionando destino aleatorio..." << std::endl;
        std::pair<int, int> random_position = get_random_position(tank.x, tank.y, 4);

        if (random_position.first != tank.x || random_position.second != tank.y) {
            std::cout << "Intentando mover aleatoriamente de (" << tank.x << ", " << tank.y << ") a (" << random_position.first << ", " << random_position.second << ")" << std::endl;
            move_tank_towards_step_by_step(tank, random_position.first, random_position.second, 1);

            // Añadir a la ruta para dibujar la traza
            int current_pos = tank.x * map->get_width() + tank.y;
            current_route.push_back(current_pos);
        }
    }

    // Incrementar el contador de intentos
    attempt_count++;

    // Redibujar el área para mostrar el trazo
    gtk_widget_queue_draw(GameArea::get_game_area());

    if (tank.x == target_x && tank.y == target_y) {
        std::cout << "Tanque alcanzó el destino: (" << target_x << ", " << target_y << ")" << std::endl;
        attempt_count = 0;  // Reiniciar el contador al alcanzar el destino
        return;
    }

    g_timeout_add(100, [](gpointer data) -> gboolean {
        Tank* tank = static_cast<Tank*>(data);
        tank->game_logic->random_move_step_by_step(*tank, tank->game_logic->aim_target_x, tank->game_logic->aim_target_y);
        return FALSE;
    }, &tank);
}




gboolean GameLogic::move_tank_step_by_step(gpointer data) {
    Tank* tank = static_cast<Tank*>(data);

    if (!tank->is_active) {
        return FALSE;  // Si el tanque ya no está activo, detener el temporizador
    }

    static int max_steps = 30;  // Máximo número de pasos permitidos por movimiento
    static int step_count = 0;

    if (step_count > max_steps) {
        std::cout << "Movimiento detenido por exceso de pasos" << std::endl;
        step_count = 0;  // Reiniciar el contador para el siguiente movimiento
        return FALSE;    // Detener el movimiento
    }

    if (!tank->route.empty()) {
        int next_pos = tank->route.front();
        tank->route.erase(tank->route.begin());  // Eliminar el primer punto de la ruta
        int new_x = next_pos / tank->game_logic->map->get_width();
        int new_y = next_pos % tank->game_logic->map->get_width();

        // Mover el tanque a la nueva posición
        tank->game_logic->move_tank(tank->id, new_x, new_y);
        step_count++;

        gtk_widget_queue_draw(GameArea::get_game_area());  // Redibujar el área de dibujo

        return TRUE;  // Continuar con el temporizador
    }

    step_count = 0;  // Reiniciar el contador al completar el movimiento
    return FALSE;  // Detener el temporizador si no hay más movimientos
}

void GameLogic::move_tank_towards_step_by_step(Tank& tank, int target_x, int target_y, int step_distance) {
    std::cout << "Antes de mover: Tank en (" << tank.x << ", " << tank.y << ") hacia destino (" << target_x << ", " << target_y << ")" << std::endl;

    int dx = (target_x > tank.x) ? step_distance : (target_x < tank.x) ? -step_distance : 0;
    int dy = (target_y > tank.y) ? step_distance : (target_y < tank.y) ? -step_distance : 0;

    // Mover en diagonal si no hay obstáculos
    if (dx != 0 && dy != 0 && !map->has_obstacle(tank.x + dx, tank.y + dy)) {
        tank.x += dx;
        tank.y += dy;
        std::cout << "Tanque movido en diagonal a (" << tank.x << ", " << tank.y << ")" << std::endl;
    }
    // Mover en la dirección de x si no hay obstáculos
    else if (dx != 0 && !map->has_obstacle(tank.x + dx, tank.y)) {
        tank.x += dx;
        std::cout << "Tanque movido en el eje X a (" << tank.x << ", " << tank.y << ")" << std::endl;
    }
    // Mover en la dirección de y si no hay obstáculos
    else if (dy != 0 && !map->has_obstacle(tank.x, tank.y + dy)) {
        tank.y += dy;
        std::cout << "Tanque movido en el eje Y a (" << tank.x << ", " << tank.y << ")" << std::endl;
    }

    // Verifica la nueva posición del tanque
    std::cout << "Después de mover: Tank en (" << tank.x << ", " << tank.y << ")" << std::endl;

    // Actualiza la posición del tanque en el mapa
    move_tank(tank.id, tank.x, tank.y);

    // Si el tanque ha alcanzado su destino, detener el movimiento
    if (tank.x == target_x && tank.y == target_y) {
        std::cout << "Tanque alcanzó el destino: (" << target_x << ", " << target_y << ")" << std::endl;
    }
}



bool GameLogic::check_line_of_sight(Tank& tank, int target_x, int target_y) {
    // Verificar si hay línea de vista desde el tanque hasta el objetivo
    int dx = target_x - tank.x;  // Diferencia en filas
    int dy = target_y - tank.y;  // Diferencia en columnas
    int steps = std::max(abs(dx), abs(dy));
    float step_x = dx / static_cast<float>(steps);
    float step_y = dy / static_cast<float>(steps);

    std::cout << "Verificando línea de vista desde (" << tank.x << ", " << tank.y << ") hacia (" << target_x << ", " << target_y << ")" << std::endl;

    for (int i = 1; i <= steps; ++i) {
        int x = static_cast<int>(tank.x + i * step_x);
        int y = static_cast<int>(tank.y + i * step_y);

        std::cout << "Verificando posición (" << x << ", " << y << ") en la línea de vista" << std::endl;

        // Asegurarse de que `x` sea la fila y `y` sea la columna
        if (map->has_obstacle(x, y)) {
            std::cout << "¡Obstáculo encontrado en (" << x << ", " << y << ")!" << std::endl;
            return false;  // Hay un obstáculo en el camino
        }
    }

    return true;  // No hay obstáculos, línea de vista clara
}


std::pair<int, int> GameLogic::get_random_position(int x, int y, int radius) {
    int rand_x = y + (std::rand() % (2 * radius + 1)) - radius;  // Intercambia x e y
    int rand_y = x + (std::rand() % (2 * radius + 1)) - radius;  // Intercambia x e y

    rand_x = std::clamp(rand_x, 0, map->get_width() - 1);
    rand_y = std::clamp(rand_y, 0, map->get_height() - 1);

    std::cout << "Posición aleatoria calculada desde (" << x << ", " << y << ") dentro de radio "
              << radius << ": (" << rand_x << ", " << rand_y << ")" << std::endl;

    return {rand_x, rand_y};
}




// En GameLogic.cpp, dentro de la función `shoot` y `update`

// GameLogic.cpp
void GameLogic::shoot(Tank& tank, int aim_target_x, int aim_target_y) {

    projectile_trail.clear();

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

    // Crear el proyectil en la posición actual del tanque y pasar el tanque que disparó
    Projectile projectile(tank_center_x, tank_center_y, dx, dy, 18.0, map->get_width() * 25, map->get_height() * 25, this, &tank);
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
                // Aquí compensamos la posición para centrar el widget con la traza
                int widget_x = proj->x - 5;  // Compensar por el radio del widget (5 píxeles)
                int widget_y = proj->y - 5;  // Compensar por el radio del widget (5 píxeles)

                gtk_fixed_move(GTK_FIXED(GameArea::get_game_area()), proj->widget, widget_x, widget_y);  // Mover en píxeles
                gtk_widget_queue_draw(proj->widget);
            }

            return TRUE;  // Continuar el temporizador mientras el proyectil esté activo
        }, &projectiles.back());
    } else {
        std::cerr << "Error: game_area no es un contenedor GtkFixed válido." << std::endl;
    }

    // Limpiar la ruta actual
    current_route.clear();

    // Redibujar el área de juego para que se borre la línea de movimiento
    gtk_widget_queue_draw(GameArea::get_game_area());

    // Cambiar el turno inmediatamente después de disparar
    end_turn();
}


void GameLogic::remove_tank(Tank& tank) {
    if (tank.widget && GTK_IS_WIDGET(tank.widget)) {
        gtk_widget_hide(tank.widget);  // Ocultar el widget en lugar de destruirlo
    }
    tank.is_active = false;  // Marcar el tanque como inactivo

}


// Luego, en la función de actualización, elimina el tanque del vector después de verificar si está destruido
void GameLogic::update_projectiles() {
    std::vector<Projectile> active_projectiles;

    for (auto& projectile : projectiles) {
        projectile.update();  // Actualizar la posición del proyectil

        if (projectile.active) {
            active_projectiles.push_back(projectile);  // Mantener proyectiles activos
        } else {
            // Limpiar las trazas del proyectil inactivo
            projectile_trail.clear();

            // Destruir el widget del proyectil si está inactivo
            if (projectile.widget && GTK_IS_WIDGET(projectile.widget)) {
                gtk_widget_destroy(projectile.widget);  // Destruir el widget del proyectil
                projectile.widget = nullptr;
            }
        }
    }

    projectiles = std::move(active_projectiles);  // Reemplazar con los proyectiles activos
}


void GameLogic::update() {
    // Actualizar proyectiles y tanques como de costumbre...
    update_projectiles();

    // Procesar eliminaciones diferidas
    process_removals();

    // Redibujar el área de juego
    gtk_widget_queue_draw(GameArea::get_game_area());
}



void GameLogic::mark_tank_for_removal(Tank* tank) {
    if (tank && tank->is_active) {
        tank->is_active = false;  // Marcar el tanque como inactivo
        tanks_to_remove.push_back(tank);  // Añadir a la lista de eliminación diferida
    }
}

void GameLogic::mark_projectile_for_removal(Projectile* projectile) {
    if (projectile && projectile->active) {
        projectile->active = false;  // Marcar el proyectil como inactivo
        projectiles_to_remove.push_back(projectile);  // Añadir a la lista de eliminación diferida
    }
}

void GameLogic::process_removals() {
    // Eliminar tanques marcados
    for (Tank* tank : tanks_to_remove) {
        if (tank->widget && GTK_IS_WIDGET(tank->widget)) {
            gtk_widget_destroy(tank->widget);  // Destruir el widget del tanque
        }
        // Opcional: eliminar el tanque del vector de tanques si es necesario
        auto it = std::remove_if(tanks.begin(), tanks.end(), [tank](const Tank& t) { return &t == tank; });
        tanks.erase(it, tanks.end());
    }
    tanks_to_remove.clear();  // Limpiar la lista de tanques eliminados

    // Eliminar proyectiles marcados
    for (Projectile* projectile : projectiles_to_remove) {
        if (projectile->widget && GTK_IS_WIDGET(projectile->widget)) {
            gtk_widget_destroy(projectile->widget);  // Destruir el widget del proyectil
        }
        // Eliminar el proyectil del vector de proyectiles
        auto it = std::remove_if(projectiles.begin(), projectiles.end(), [projectile](const Projectile& p) { return &p == projectile; });
        projectiles.erase(it, projectiles.end());
    }
    projectiles_to_remove.clear();  // Limpiar la lista de proyectiles eliminados
}


std::vector<Tank>& GameLogic::get_tanks() {
    return tanks;  // Devolver referencia no constante
}

Map* GameLogic::get_map() const {
    return map;
}
