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
     * @brief Reproduce la m�sica de fondo de forma controlada.
     *
     * Solo inicia la m�sica si no se est� reproduciendo ya el mismo tema.
     * Adem�s, marca que en este ciclo se desea la reproducci�n.
     *
     * @param etiqueta La etiqueta de la m�sica.
     * @param loops N�mero de repeticiones: 0 reproduce una vez, -1 para bucle infinito.
     */
    void reproducirControlado(const std::string& etiqueta, int loops = 0) {
        // Marca que se desea mantener la reproducci�n en este ciclo.
        activo = true;

        // Si ya se est� reproduciendo el mismo sonido, no hacemos nada.
        if (Mix_PlayingMusic() && sonidoActual == etiqueta) {
            return;
        }

        // Si hay otra m�sica reproduci�ndose, se detiene abruptamente.
        if (Mix_PlayingMusic()) {
            Mix_FadeOutMusic(0);  // O usar Mix_HaltMusic() para un corte inmediato.
        }

        // Inicia la nueva m�sica.
        Sonido::ObtenerInstancia().reproducirMusica(etiqueta, loops);
        sonidoActual = etiqueta;
    }

    /**
     * @brief M�todo de actualizaci�n que debe llamarse en cada ciclo.
     *
     * Si en el ciclo actual no se ha pedido reproducirControlado, se detiene la m�sica.
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
    bool activo;  // Indica si se ha pedido reproducci�n en el ciclo actual.
};

#endif // CONTROLSONIDOS_H