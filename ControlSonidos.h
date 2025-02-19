// ControlSonidos.h
#ifndef CONTROLSONIDOS_H
#define CONTROLSONIDOS_H

#include <string>
#include <iostream>
#include <SDL_mixer.h>
#include "Sonido.h"

class ControlSonidos {
public:
    ControlSonidos() : sonidoActual(""), activo(false) {}
    ~ControlSonidos() {}

    /**
     * @brief Reproduce la música de fondo de forma controlada.
     *
     * Solo inicia la música si no se está reproduciendo ya el mismo tema.
     * Además, marca que en este ciclo se desea la reproducción.
     *
     * @param etiqueta La etiqueta de la música.
     * @param loops Número de repeticiones: 0 reproduce una vez, -1 para bucle infinito.
     */
    void reproducirControlado(const std::string& etiqueta, int loops = 0) {
        // Marca que se desea mantener la reproducción en este ciclo.
        activo = true;

        // Si ya se está reproduciendo el mismo sonido, no hacemos nada.
        if (Mix_PlayingMusic() && sonidoActual == etiqueta) {
            return;
        }

        // Si hay otra música reproduciéndose, se detiene abruptamente.
        if (Mix_PlayingMusic()) {
            Mix_FadeOutMusic(0);  // O usar Mix_HaltMusic() para un corte inmediato.
        }

        // Inicia la nueva música.
        Sonido::ObtenerInstancia().reproducirMusica(etiqueta, loops);
        sonidoActual = etiqueta;
    }

    /**
     * @brief Método de actualización que debe llamarse en cada ciclo.
     *
     * Si en el ciclo actual no se ha pedido reproducirControlado, se detiene la música.
     */
    void Actualizar() {
        if (!activo) {
            if (Mix_PlayingMusic()) {
                Mix_HaltMusic();
                sonidoActual = "";
            }
        }
        // Reiniciamos el flag para el siguiente ciclo.
        activo = false;
    }

private:
    std::string sonidoActual;
    bool activo;  // Indica si se ha pedido reproducción en el ciclo actual.
};

#endif // CONTROLSONIDOS_H