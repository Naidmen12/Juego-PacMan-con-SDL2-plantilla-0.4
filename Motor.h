#ifndef MOTOR_H
#define MOTOR_H

#include <SDL.h>
#include <iostream>

class Motor {
public:
    Motor(int fps);
    ~Motor();

    bool corriendo;
    void Inicializar();
    void Eventos();
    void Actualizar();
    void Imprimir();
    void ControlarFrames();
    void EliminarTodo();
    void EliminarObjetos();

private:
    SDL_Window* ventana;
    SDL_Renderer* renderizador;
    SDL_Event evento;
    const int FPS;
    Uint32 tiempoAnterior;
    float deltaTime;
    float tiempoAcumulado;
    int Estado, Recorrido;
};

#endif // MOTOR_H
