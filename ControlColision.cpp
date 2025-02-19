#include "ControlColision.h"
#include <cmath>
#include <algorithm>

ControlColision::ControlColision() : Contador(0) {
}

LadoColision ControlColision::ComprobarColision(Entidad& entidad, const Mapa& mapa, int gid) {
    const auto& matrizTiles = mapa.ObtenerMatrizTiles();
    SDL_Rect entidadRect = entidad.ObtenerDestRect();
    Vector2D velocidad = entidad.ObtenerVelocidad();
    LadoColision ladoColision = LadoColision::Ninguno;

    // Crear rectángulos para la próxima posición en cada eje (sin margen)
    SDL_Rect nextPosX = { entidadRect.x + static_cast<int>(velocidad.x), entidadRect.y, entidadRect.w, entidadRect.h };
    SDL_Rect nextPosY = { entidadRect.x, entidadRect.y + static_cast<int>(velocidad.y), entidadRect.w, entidadRect.h };

    for (const auto& fila : matrizTiles) {
        for (const auto& tile : fila) {
            if (tile.gid == gid) { // Solo realizar la colisión si el gid del tile es igual al proporcionado
                SDL_Rect tileRect = tile.destRect;

                // Comprobar colisión en el eje X (sin margen)
                if (SDL_HasIntersection(&nextPosX, &tileRect)) {
                    if (velocidad.x > 0) {
                        entidadRect.x = tileRect.x - entidadRect.w;
                        ladoColision = LadoColision::Derecha;
                    }
                    if (velocidad.x < 0) {
                        entidadRect.x = tileRect.x + tileRect.w;
                        ladoColision = LadoColision::Izquierda;
                    }
                    velocidad.x = 0;
                }

                // Comprobar colisión en el eje Y (sin margen)
                if (SDL_HasIntersection(&nextPosY, &tileRect)) {
                    if (velocidad.y > 0) {
                        entidadRect.y = tileRect.y - entidadRect.h;
                        ladoColision = LadoColision::Abajo;
                    }
                    if (velocidad.y < 0) {
                        entidadRect.y = tileRect.y + tileRect.h;
                        ladoColision = LadoColision::Arriba;
                    }
                    velocidad.y = 0;
                }
            }
        }
    }

    // Ajustar la posición final después de manejar colisiones
    entidad.EstablecerPosicion(entidadRect.x, entidadRect.y);
    entidad.EstablecerVelocidad(velocidad.x, velocidad.y);

    return ladoColision;
}

void ControlColision::ColisionEQ(Entidad* entidad, Mapa* mapa, int gid) {
    auto& matrizTiles = mapa->ObtenerMatrizTiles();
    SDL_Rect entidadRect = entidad->ObtenerDestRect();

    float entidadCentroX = entidadRect.x + entidadRect.w / 2.0f;
    float entidadCentroY = entidadRect.y + entidadRect.h / 2.0f;
    float entidadRadio = std::min(entidadRect.w, entidadRect.h) / 2.0f;

    const float factorReduccionTile = 0.6f;

    bool colisionDetectada = false;

    for (auto& fila : matrizTiles) {
        for (auto& tile : fila) {
            if (tile.gid == gid) {
                SDL_Rect tileRect = tile.destRect;
                float tileCentroX = tileRect.x + tileRect.w / 2.0f;
                float tileCentroY = tileRect.y + tileRect.h / 2.0f;
                float tileRadio = (std::min(tileRect.w, tileRect.h) / 2.0f) * factorReduccionTile;

                float deltaX = entidadCentroX - tileCentroX;
                float deltaY = entidadCentroY - tileCentroY;
                float distancia = std::sqrt(deltaX * deltaX + deltaY * deltaY);

                if (distancia < (entidadRadio + tileRadio)) {
                    tile.gid = 0;
                    tile.srcRect = { 0, 0, 0, 0 };
                    tile.destRect = { 0, 0, 0, 0 };
                    Contador++;
                    colisionDetectada = true;
                    break;
                }
            }
        }
        if (colisionDetectada) {
            break;
        }
    }
}

bool ControlColision::ColisionEnemigo(Entidad& entidad, Enemigo& enemigo) {
    SDL_Rect rectEntidad = entidad.ObtenerDestRect();
    SDL_Rect rectEnemigo = enemigo.ObtenerDestRect();
    return SDL_HasIntersection(&rectEntidad, &rectEnemigo);
}

int ControlColision::ObtenerContador() const {
    return Contador;
}