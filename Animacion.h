#ifndef ANIMACION_H
#define ANIMACION_H

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Animacion {
public:
    // Se elimina el parámetro filaFija del constructor
    Animacion(const std::string& idTextura, SDL_Rect rectFrame, int retardoFrames);

    // Ahora actualizar recibe el parámetro filaFija
    SDL_Rect actualizar(int filaFija);

    void setRetardoFrames(int nuevoRetardo);
    int getRetardoFrames() const;
    void reiniciar();
    SDL_Texture* obtenerTextura() const;

private:
    void obtenerCaracteristicasTextura();

    std::string idTextura;
    SDL_Texture* textura;
    SDL_Rect rectFrameActual;
    int retardoFrames;
    int contadorFrames;
    int columnas;
    int filas;
};

#endif // ANIMACION_H
