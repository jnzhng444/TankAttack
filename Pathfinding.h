#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <vector>

// Declaración de la clase Pathfinding
class Map;  // Forward declaration de Map

class Pathfinding {
public:
    // Función de búsqueda BFS
    static std::vector<int> bfs(Map* map, int start_x, int start_y, int goal_x, int goal_y);

    // Función de búsqueda Dijkstra
    static std::vector<int> dijkstra(Map* map, int start_x, int start_y, int goal_x, int goal_y);

    //Funcion de busqueda A*
    static std::vector<int> a_star(Map* map, int start_x, int start_y, int goal_x, int goal_y);
};

#endif // PATHFINDING_H
