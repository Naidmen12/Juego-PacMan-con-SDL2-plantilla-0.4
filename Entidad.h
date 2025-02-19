#ifndef ENTIDAD_H
#define ENTIDAD_H

#include "Objeto.h"
#include "Vector2D.h"
#include <string>

class Entidad : public Objeto {
public:
    Entidad(std::string idTextura, SDL_Rect desRect, SDL_Rect srcRect);

    void Actualizar();
    void Mover(float dx, float dy);
    void EstablerLimiteMovimiento(bool izquierda, bool derecha, bool arriba, bool abajo);

    void EstablecerPosicion(float x, float y);
    Vector2D ObtenerPosicion() const;

    void EstablecerVelocidad(float vx, float vy);
    Vector2D ObtenerVelocidad() const;

    SDL_Rect ObtenerDestRect() const; // Método agregado
    SDL_Rect ObtenerScrRect(); // Método agregado
    void EstablecerScrRect(const SDL_Rect& nuevoScrRect);

private:
    Vector2D posicion;
    Vector2D velocidad;

    bool puedeMoverIzquierda = true;
    bool puedeMoverDerecha = true;
    bool puedeMoverArriba = true;
    bool puedeMoverAbajo = true;
};

#endif // ENTIDAD_H