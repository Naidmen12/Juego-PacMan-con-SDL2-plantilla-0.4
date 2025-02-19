#ifndef OBJETO_H
#define OBJETO_H

#include "Vector2D.h"
#include "ManejarTextura.h"
#include <string>
#include <SDL.h>

class Objeto {
public:
    // Constructor
    Objeto(const std::string& idTextura, const SDL_Rect& desRect, const SDL_Rect& srcRect);

    // Método virtual que se puede sobreescribir, pero que por defecto no hace nada
    virtual void Actualizar();

    // Dibuja el objeto usando la textura asociada
    virtual void Dibujar(SDL_Renderer* renderizador);

    // Libera los recursos asociados (en este ejemplo, limpia todas las texturas gestionadas)
    virtual void Limpiar();

    // Destructor virtual
    virtual ~Objeto();

protected:
    std::string idTextura;
    SDL_Rect srcRect;
    SDL_Rect destRect;
};

#endif // OBJETO_H

