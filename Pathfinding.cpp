#include "Pathfinding.h"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>

// Estructura interna para los nodos del algoritmo A*
struct Node {
    int x, y;         // Coordenadas en la cuadrícula (índices de tile)
    float g, h;       // g: costo desde el inicio, h: heurística (estimación al destino)
    Node* parent;     // Nodo padre en el camino

    Node(int _x, int _y, float _g, float _h, Node* _parent = nullptr)
        : x(_x), y(_y), g(_g), h(_h), parent(_parent) {
    }

    float f() const { return g + h; }
};

// Heurística: distancia Manhattan
static float heuristic(int x1, int y1, int x2, int y2) {
    return static_cast<float>(abs(x1 - x2) + abs(y1 - y2));
}

std::vector<Vector2D> Pathfinding::buscarCaminoAStar(const Mapa& mapa, Vector2D inicio, Vector2D destino, int gidBloqueador) {
    std::vector<Vector2D> path;

    // Obtener la matriz de tiles
    const auto& matriz = mapa.ObtenerMatrizTiles();
    int rows = static_cast<int>(matriz.size());
    if (rows == 0) return path;
    int cols = static_cast<int>(matriz[0].size());
    if (cols == 0) return path;

    // Se asume que el tamaño de cada tile es el ancho (destRect.w) del primer tile.
    int tileSize = matriz[0][0].destRect.w;

    // Convertir posiciones en píxeles a coordenadas de la cuadrícula.
    int startX = static_cast<int>(inicio.x) / tileSize;
    int startY = static_cast<int>(inicio.y) / tileSize;
    int endX = static_cast<int>(destino.x) / tileSize;
    int endY = static_cast<int>(destino.y) / tileSize;

    // Verificar límites
    if (startX < 0 || startY < 0 || startX >= cols || startY >= rows ||
        endX < 0 || endY < 0 || endX >= cols || endY >= rows)
        return path;

    // Lambda para determinar si un tile es transitable (no bloqueador)
    auto esTransitable = [&](int x, int y) -> bool {
        return matriz[y][x].gid != gidBloqueador;
        };

    // Listas para el algoritmo A*
    std::vector<Node*> openList;
    std::vector<std::vector<bool>> closed(rows, std::vector<bool>(cols, false));

    Node* startNode = new Node(startX, startY, 0.0f, heuristic(startX, startY, endX, endY));
    openList.push_back(startNode);
    Node* goalNode = nullptr;

    while (!openList.empty()) {
        // Seleccionar el nodo con menor f()
        auto currentIt = openList.begin();
        Node* current = *currentIt;
        for (auto it = openList.begin(); it != openList.end(); ++it) {
            if ((*it)->f() < current->f()) {
                current = *it;
            }
        }

        // Remover current de openList y marcarlo como cerrado.
        openList.erase(std::remove(openList.begin(), openList.end(), current), openList.end());
        closed[current->y][current->x] = true;

        // Si llegamos al destino, terminamos.
        if (current->x == endX && current->y == endY) {
            goalNode = current;
            break;
        }

        // Vecinos en 4 direcciones: arriba, abajo, izquierda, derecha.
        std::vector<std::pair<int, int>> vecinos = {
            {current->x + 1, current->y},
            {current->x - 1, current->y},
            {current->x, current->y + 1},
            {current->x, current->y - 1}
        };

        for (auto& vecino : vecinos) {
            int nx = vecino.first, ny = vecino.second;
            if (nx < 0 || ny < 0 || nx >= cols || ny >= rows)
                continue;
            if (!esTransitable(nx, ny))
                continue;
            if (closed[ny][nx])
                continue;

            float newG = current->g + 1.0f;  // costo unitario
            // Buscar si el vecino ya está en openList.
            Node* neighbor = nullptr;
            for (auto node : openList) {
                if (node->x == nx && node->y == ny) {
                    neighbor = node;
                    break;
                }
            }
            if (neighbor == nullptr) {
                neighbor = new Node(nx, ny, newG, heuristic(nx, ny, endX, endY), current);
                openList.push_back(neighbor);
            }
            else if (newG < neighbor->g) {
                neighbor->g = newG;
                neighbor->parent = current;
            }
        }
    }

    // Si se encontró un camino, se reconstruye.
    if (goalNode != nullptr) {
        Node* nodo = goalNode;
        while (nodo != nullptr) {
            // Convertir coordenadas de cuadrícula a posición en píxeles (centro del tile)
            float posX = nodo->x * tileSize + tileSize / 2.0f;
            float posY = nodo->y * tileSize + tileSize / 2.0f;
            path.push_back(Vector2D(posX, posY));
            nodo = nodo->parent;
        }
        std::reverse(path.begin(), path.end());
    }

    // Liberar memoria de los nodos en openList
    for (auto node : openList) {
        delete node;
    }
    // Nota: En una implementación completa, se debe liberar toda la memoria asignada.

    return path;
}
