#include <string>
#include <queue>
#include <cstdlib>
#include <vector>

class PowerUp {
public:
    enum Type {
        DobleTurno,
        PrecisionMovimiento,
        PrecisionAtaque,
        PoderAtaque
    };

    PowerUp(Type type) : type(type) {}
    PowerUp() : type(DobleTurno) {} // Constructor predeterminado

    Type type;
    bool isActive = false; // Indica si el power-up está activo
    int turnsRemaining = 0; // Turnos restantes para el power-up
};