#include "Objeto.h"
#include <iostream>
#include "ManejarTextura.h"

Objeto::Objeto(const std::string& idTextura, const SDL_Rect& desRect, const SDL_Rect& srcRect)
    : idTextura(idTextura), srcRect(srcRect), destRect(desRect) {
}

void Objeto::Actualizar() {
    // Implementaci�n por defecto (no hace nada)
}

void Objeto::Dibujar(SDL_Renderer* renderizador) {
    SDL_Texture* textura = ManejarTextura::Instancia()->ObtenerTextura(idTextura);
    if (textura) {
        SDL_RenderCopy(renderizador, textura, &srcRect, &destRect);
    }
    else {
        std::cerr << "Textura no encontrada para ID: " << idTextura << std::endl;
    }
}

void Objeto::Limpiar() {
    // Se puede dejar vac�a o, si se desea, limpiar texturas administradas (esto depender� de la l�gica de tu aplicaci�n)
    ManejarTextura::Instancia()->Limpiar();
}

Objeto::~Objeto() {
    // Se pueden liberar recursos propios aqu� si fuera necesario
}