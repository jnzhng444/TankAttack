#include "Pathfinding.h"
#include "Map.h"  // Asegúrate de tener la definición de Map
#include <queue>
#include <vector>
#include <limits>
#include <algorithm>

// Implementación de BFS
std::vector<int> Pathfinding::bfs(Map* map, int start_x, int start_y, int goal_x, int goal_y) {
    int width = map->get_width();
    int height = map->get_height();

    std::vector<std::vector<int>> dist(width, std::vector<int>(height, -1));
    std::vector<std::vector<int>> prev(width, std::vector<int>(height, -1));

    std::queue<std::pair<int, int>> q;
    std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    q.push({start_x, start_y});
    dist[start_x][start_y] = 0;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        if (x == goal_x && y == goal_y) {
            std::vector<int> path;
            while (x != start_x || y != start_y) {
                path.push_back(x * width + y);
                int p = prev[x][y];
                x = p / width;
                y = p % width;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& [dx, dy] : directions) {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && ny >= 0 && nx < width && ny < height && dist[nx][ny] == -1 && !map->has_obstacle(nx, ny)) {
                q.push({nx, ny});
                dist[nx][ny] = dist[x][y] + 1;
                prev[nx][ny] = x * width + y;
            }
        }
    }

    return {};
}

// Implementación de Dijkstra
std::vector<int> Pathfinding::dijkstra(Map* map, int start_x, int start_y, int goal_x, int goal_y) {
    int width = map->get_width();
    int height = map->get_height();

    std::vector<std::vector<int>> dist(width, std::vector<int>(height, std::numeric_limits<int>::max()));
    std::vector<std::vector<int>> prev(width, std::vector<int>(height, -1));

    std::priority_queue<std::pair<int, std::pair<int, int>>, std::vector<std::pair<int, std::pair<int, int>>>, std::greater<>> pq;
    std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    pq.push({0, {start_x, start_y}});
    dist[start_x][start_y] = 0;

    while (!pq.empty()) {
        auto [current_dist, pos] = pq.top();
        int x = pos.first, y = pos.second;
        pq.pop();

        if (x == goal_x && y == goal_y) {
            std::vector<int> path;
            while (x != start_x || y != start_y) {
                path.push_back(x * width + y);
                int p = prev[x][y];
                x = p / width;
                y = p % width;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        if (current_dist > dist[x][y]) {
            continue;
        }

        for (const auto& [dx, dy] : directions) {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && ny >= 0 && nx < width && ny < height && !map->has_obstacle(nx, ny)) {
                int new_dist = current_dist + 1;
                if (new_dist < dist[nx][ny]) {
                    dist[nx][ny] = new_dist;
                    prev[nx][ny] = x * width + y;
                    pq.push({new_dist, {nx, ny}});
                }
            }
        }
    }

    return {};
}

// Heurística de la distancia Manhattan
int heuristic(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

// Implementación de A*
std::vector<int> Pathfinding::a_star(Map* map, int start_x, int start_y, int goal_x, int goal_y) {
    int width = map->get_width();
    int height = map->get_height();

    std::vector<std::vector<int>> g_score(width, std::vector<int>(height, std::numeric_limits<int>::max()));
    std::vector<std::vector<int>> f_score(width, std::vector<int>(height, std::numeric_limits<int>::max()));
    std::vector<std::vector<std::pair<int, int>>> came_from(width, std::vector<std::pair<int, int>>(height, {-1, -1}));

    g_score[start_x][start_y] = 0;
    f_score[start_x][start_y] = heuristic(start_x, start_y, goal_x, goal_y);

    std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, std::greater<>> open_set;
    open_set.emplace(f_score[start_x][start_y], start_x, start_y);

    std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    while (!open_set.empty()) {
        auto [current_f, x, y] = open_set.top();
        open_set.pop();

        if (x == goal_x && y == goal_y) {
            std::vector<int> path;
            while (!(x == start_x && y == start_y)) {
                path.push_back(x * width + y);
                auto [prev_x, prev_y] = came_from[x][y];
                x = prev_x;
                y = prev_y;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& [dx, dy] : directions) {
            int nx = x + dx, ny = y + dy;

            if (nx >= 0 && ny >= 0 && nx < width && ny < height) {
                int tentative_g_score = g_score[x][y] + 1; // Costo de mover a una celda adyacente

                if (tentative_g_score < g_score[nx][ny]) {
                    came_from[nx][ny] = {x, y};
                    g_score[nx][ny] = tentative_g_score;
                    f_score[nx][ny] = tentative_g_score + heuristic(nx, ny, goal_x, goal_y);

                    open_set.emplace(f_score[nx][ny], nx, ny);
                }
            }
        }
    }

    return {}; // Devuelve una lista vacía si no se encuentra un camino
}
