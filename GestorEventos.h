#ifndef GESTOREVENTOS_H
#define GESTOREVENTOS_H

#include <SDL.h>
#include "Entidad.h"
#include "Animacion.h" // Incluir la clase Animacion
#include "Menus.h"     // Incluir Menus
#include "Sonido.h"

class GestorEventos {
public:
    // El constructor recibe punteros a Entidad, Animacion y Menus (que puede ser nullptr).
    GestorEventos(Entidad* entidad, Animacion* animacion, Menus* menu);

    // Maneja otros eventos (movimiento y animaci�n) para la entidad.
    void ManejarEventos();

    // TecladoMenu(): se debe llamar en cada frame desde Actualizar().
    // Devuelve:
    //   - -2 si el usuario a�n no ha confirmado la opci�n (no se ha presionado ENTER).
    //   - El valor de la opci�n seleccionada cuando se presiona ENTER.
    int TecladoMenu();

private:
    Entidad* entidad;
    Animacion* animacion;
    Menus* menu;             // Puntero a la instancia de Menus (puede ser nullptr).
    float velocidadMovimiento;
    int filaAnimacion;

    // Variables para manejo del men� de forma no bloqueante
    bool menuFinished;  // Indica si ya se confirm� la selecci�n.
    bool prevUp, prevDown, prevLeft, prevRight, prevEnter; // Estados previos para edge detection.
    int menuSelection;  // Almacena la opci�n seleccionada cuando se confirma.
};

#endif // GESTOREVENTOS_H