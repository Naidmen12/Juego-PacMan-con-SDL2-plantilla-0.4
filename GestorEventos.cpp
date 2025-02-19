#include "GestorEventos.h"
#include <SDL.h>
#include <iostream>

GestorEventos::GestorEventos(Entidad* entidad, Animacion* animacion, Menus* menu)
    : entidad(entidad), animacion(animacion), menu(menu),
    velocidadMovimiento(2.0f), filaAnimacion(0),
    menuFinished(false), prevUp(false), prevDown(false),
    prevLeft(false), prevRight(false), prevEnter(false),
    menuSelection(-2){
}

#include "Sonido.h"
#include <SDL.h>

void GestorEventos::ManejarEventos(/* ... otros par�metros si los hay ... */) {
    const Uint8* keyState = SDL_GetKeyboardState(NULL);

    // Calcular movimiento en X e Y.
    float dx = (keyState[SDL_SCANCODE_RIGHT] - keyState[SDL_SCANCODE_LEFT]) * velocidadMovimiento;
    float dy = (keyState[SDL_SCANCODE_DOWN] - keyState[SDL_SCANCODE_UP]) * velocidadMovimiento;

    // Determinar la fila de animaci�n seg�n la direcci�n.
    if (dx > 0) {
        filaAnimacion = 0;
    }
    else if (dx < 0) {
        filaAnimacion = 1;
    }
    else if (dy < 0) {
        filaAnimacion = 2;
    }
    else if (dy > 0) {
        filaAnimacion = 3;
    }

    // Actualizar animaci�n y mover la entidad si existe.
    if (dx != 0 || dy != 0) {
        SDL_Rect nuevoSrcRect = animacion->actualizar(filaAnimacion);
        if (entidad) {
            entidad->EstablecerScrRect(nuevoSrcRect);
        }
        // Si se est� moviendo, reproducir "Waka" en canal 1 (si a�n no se est� reproduciendo).
        if (!Mix_Playing(1)) {
            // El par�metro loops = -1 har� que el efecto se repita continuamente.
            Sonido::ObtenerInstancia().reproducirEfecto("Waka", -1, 1);
        }
    }
    else {
        // Si no hay movimiento, detener el canal 1 donde se reproduce "Waka".
        if (Mix_Playing(1)) {
            Mix_HaltChannel(1);
        }
    }

    if (entidad) {
        entidad->Mover(dx, dy);
    }
}

int GestorEventos::TecladoMenu() {
    // Si ya se complet� la selecci�n, devolvemos el valor almacenado.
    if (menuFinished) {
        return menuSelection;
    }

    // Si hay un men�, activamos el resaltado (esto se puede hacer una sola vez).
    if (menu) {
        menu->activarResaltado();
    }

    // Obtener los estados actuales del teclado.
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    bool currentUp = keys[SDL_SCANCODE_UP];
    bool currentDown = keys[SDL_SCANCODE_DOWN];
    bool currentLeft = keys[SDL_SCANCODE_LEFT];
    bool currentRight = keys[SDL_SCANCODE_RIGHT];
    bool currentEnter = keys[SDL_SCANCODE_RETURN];

    // Procesar la navegaci�n si existe men�.
    if (menu) {
        if (menu->obtenerModo() == ModoMenu::LISTA) {
            if (currentUp && !prevUp) {
                menu->seleccionarAnterior();
            }
            if (currentDown && !prevDown) {
                menu->seleccionarSiguiente();
            }
        }
        else { // Modo UNICO: navegaci�n con IZQUIERDA y DERECHA.
            if (currentLeft && !prevLeft) {
                menu->seleccionarAnterior();
            }
            if (currentRight && !prevRight) {
                menu->seleccionarSiguiente();
            }
        }
    }

    // Si se detecta un flanco de ENTER (pulsaci�n nueva).
    if (currentEnter && !prevEnter) {
        if (menu) {
            menu->desactivarResaltado();
            menuSelection = menu->obtenerValorSeleccionado();
        }
        else {
            menuSelection = -1;
        }
        menuFinished = true;
    }

    // Actualizar los estados previos para la siguiente llamada.
    prevUp = currentUp;
    prevDown = currentDown;
    prevLeft = currentLeft;
    prevRight = currentRight;
    prevEnter = currentEnter;

    // Mientras no se haya confirmado la selecci�n, retornamos -2.
    return 0;
}