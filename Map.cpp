#include "Map.h"
#include <iostream>
#include <queue>

Map::Map(int width, int height) : width(width), height(height) {
    // Inicializar la matriz de adyacencia para el nuevo tamaño
    adj_matrix.resize(width * height, std::vector<int>(width * height, 0));

    // Inicializar la matriz de obstáculos para el nuevo tamaño
    obstacles.resize(width, std::vector<bool>(height, false));

    std::srand(std::time(0));  // Inicializar el generador aleatorio
}

void Map::generate_random_map() {
    int min_obstacles = 70;  // Número mínimo de bloques totales
    int max_obstacles = 90;  // Número máximo de bloques totales
    int num_obstacles = 0;

    // Generar un número aleatorio de bloques entre min_obstacles y max_obstacles
    int num_to_generate = std::rand() % (max_obstacles - min_obstacles + 1) + min_obstacles;

    while (num_obstacles < num_to_generate) {
        int i = std::rand() % width;
        int j = std::rand() % height;

        // Asegurarnos de que la celda seleccionada esté vacía
        if (!obstacles[i][j]) {
            // Generar diferentes tipos de formas de obstáculos
            int shape = std::rand() % 4;  // Forma aleatoria: 0 = línea horizontal, 1 = línea vertical, 2 = forma en L, 3 = cuadrado de 2x2

            if (shape == 0) {
                // Colocar una línea horizontal de 2 a 4 bloques
                int length = 2 + std::rand() % 3;  // Longitud aleatoria entre 2 y 4 bloques
                for (int l = 0; l < length && j + l < width; ++l) {
                    if (!obstacles[i][j + l]) {
                        obstacles[i][j + l] = true;
                        num_obstacles++;
                    }
                }
            }
            else if (shape == 1) {
                // Colocar una línea vertical de 2 a 4 bloques
                int length = 2 + std::rand() % 3;  // Longitud aleatoria entre 2 y 4 bloques
                for (int l = 0; l < length && i + l < height; ++l) {
                    if (!obstacles[i + l][j]) {
                        obstacles[i + l][j] = true;
                        num_obstacles++;
                    }
                }
            }
            else if (shape == 2) {
                // Colocar una forma en "L" de 3 bloques
                obstacles[i][j] = true;
                num_obstacles++;
                if (i + 1 < height && !obstacles[i + 1][j]) {
                    obstacles[i + 1][j] = true;
                    num_obstacles++;
                }
                if (j + 1 < width && !obstacles[i][j + 1]) {
                    obstacles[i][j + 1] = true;
                    num_obstacles++;
                }
            }
            else if (shape == 3) {
                // Colocar un cuadrado de 2x2 bloques
                if (i + 1 < height && j + 1 < width) {
                    obstacles[i][j] = true;
                    obstacles[i + 1][j] = true;
                    obstacles[i][j + 1] = true;
                    obstacles[i + 1][j + 1] = true;
                    num_obstacles += 4;
                }
            }
        }
    }

    // Asegurar que el mapa sea accesible
    ensure_accessibility();
}




void Map::ensure_accessibility() {
    // Usamos BFS para asegurarnos de que todas las celdas sean accesibles
    std::vector<bool> visited(width * height, false);
    std::queue<int> queue;
    queue.push(0);  // Comenzamos desde el punto (0,0)
    visited[0] = true;

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        // Recorremos los vecinos
        for (int i = 0; i < width * height; ++i) {
            if (adj_matrix[current][i] == 1 && !visited[i]) {
                visited[i] = true;
                queue.push(i);
            }
        }
    }

    // Si alguna celda no fue visitada, la conectamos manualmente
    for (int i = 0; i < width * height; ++i) {
        if (!visited[i]) {
            // Conectar con alguna celda vecina de forma forzada
            for (int j = 0; j < width * height; ++j) {
                if (std::abs(i - j) == 1 || std::abs(i - j) == width) {
                    adj_matrix[i][j] = adj_matrix[j][i] = 1;
                    break;
                }
            }
        }
    }
}

bool Map::has_obstacle(int x, int y) {
    return obstacles[x][y];
}

void Map::print_map() {
    // Para depurar: Imprime el mapa como una cuadrícula
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::cout << (has_obstacle(i, j) ? "X " : "O ");
        }
        std::cout << std::endl;
    }
}

bool Map::is_accessible(int x, int y) {
    if (x < 0 || x >= height || y < 0 || y >= width) {
        return false; // Verificar límites
    }
    int index = x * width + y;
    for (int i = 0; i < width * height; ++i) {
        if (adj_matrix[index][i] == 1) {
            return true;
        }
    }
    return false;
}
