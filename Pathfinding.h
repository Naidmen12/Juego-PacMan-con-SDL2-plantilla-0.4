#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <vector>
#include "Vector2D.h"
#include "Mapa.h"

class Pathfinding {
public:
    static std::vector<Vector2D> buscarCaminoAStar(const Mapa& mapa, Vector2D inicio, Vector2D destino, int gidBloqueador);
};

#endif // PATHFINDING_H


