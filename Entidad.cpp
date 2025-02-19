#include "Entidad.h"
#include "ManejarTextura.h"

Entidad::Entidad(std::string idTextura, SDL_Rect desRect, SDL_Rect srcRect)
    : Objeto(idTextura, desRect, srcRect), posicion(desRect.x, desRect.y), velocidad(0, 0) {
}

void Entidad::Actualizar() {
    // Actualiza la posición de acuerdo con la velocidad
    posicion.x += velocidad.x;
    posicion.y += velocidad.y;

    // Actualiza la posición del rectángulo de destino para renderizado
    destRect.x = static_cast<int>(posicion.x);
    destRect.y = static_cast<int>(posicion.y);
}

void Entidad::Mover(float dx, float dy) {
    velocidad.x = (dx < 0 && !puedeMoverIzquierda) || (dx > 0 && !puedeMoverDerecha) ? 0 : dx;
    velocidad.y = (dy < 0 && !puedeMoverArriba) || (dy > 0 && !puedeMoverAbajo) ? 0 : dy;
}

void Entidad::EstablerLimiteMovimiento(bool izquierda, bool derecha, bool arriba, bool abajo) {
    puedeMoverIzquierda = izquierda;
    puedeMoverDerecha = derecha;
    puedeMoverArriba = arriba;
    puedeMoverAbajo = abajo;
}

void Entidad::EstablecerPosicion(float x, float y) {
    posicion.x = x;
    posicion.y = y;
}

void Entidad::EstablecerVelocidad(float vx, float vy) {
    velocidad.x = vx;
    velocidad.y = vy;
}

Vector2D Entidad::ObtenerPosicion() const {
    return posicion;
}

Vector2D Entidad::ObtenerVelocidad() const {
    return velocidad;
}

SDL_Rect Entidad::ObtenerDestRect() const {
    return destRect;
}

SDL_Rect Entidad::ObtenerScrRect(){
    return srcRect;
}

void Entidad::EstablecerScrRect(const SDL_Rect& nuevoScrRect) {
    srcRect = nuevoScrRect;
}
