#ifndef MAP_H
#define MAP_H

#include <vector>
#include <cstdlib>
#include <ctime>

class Map {
public:
    Map(int width, int height);
    void generate_random_map();
    void print_map();  // Para debug: Imprime el mapa en consola

    bool is_accessible(int x, int y);  // Chequea si una celda es accesible
    bool has_obstacle(int x, int y);   // Chequea si una celda tiene un obstáculo
    std::vector<std::vector<int>> get_adj_matrix() const;
    int get_width() const { return width; }  // Método para obtener width
    int get_height() const { return height; }  // Método para obtener height

private:
    int width, height;
    std::vector<std::vector<int>> adj_matrix;  // Matriz de adyacencia
    std::vector<std::vector<bool>> obstacles;  // Nueva matriz para obstáculos
    void ensure_accessibility();  // Garantiza que todos los puntos del mapa sean accesibles

};

#endif // MAP_H
