// Projectile.h
#ifndef PROJECTILE_H
#define PROJECTILE_H

class Projectile {
public:
    int x, y; // Posición del proyectil
    double direction_x, direction_y; // Dirección del proyectil
    double speed; // Velocidad del proyectil

    Projectile(int start_x, int start_y, double dir_x, double dir_y, double spd)
        : x(start_x), y(start_y), direction_x(dir_x), direction_y(dir_y), speed(spd) {}

    void update() {
        x += static_cast<int>(direction_x * speed);
        y += static_cast<int>(direction_y * speed);
    }
};

#endif // PROJECTILE_H
