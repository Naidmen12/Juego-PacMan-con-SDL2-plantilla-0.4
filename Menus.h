#ifndef MENUS_H
#define MENUS_H

#include "ManejoTexto.h"
#include "Objeto.h"
#include <vector>
#include <string>
#include <SDL.h>

// Enumerador para los modos de visualizaci�n del men�
enum class ModoMenu {
    UNICO, // Se muestra solo la opci�n seleccionada (navegaci�n con IZQ/DER)
    LISTA  // Se muestra la lista completa (navegaci�n con ARRIBA/ABAJO)
};

class Menus : public ManejoTexto {
public:
    // Constructor: se pasa el renderer, la etiqueta de la fuente, el rect�ngulo base y el color.
    Menus(SDL_Renderer* renderer, const std::string& fontLabel, const SDL_Rect& rect, SDL_Color color);

    // Destructor
    ~Menus();

    // Agrega una opci�n al men�.
    // El par�metro 'variable' es opcional (por defecto -1).
    void agregarOpcion(const std::string& opcion, int variable = -1);

    // Navega a la siguiente opci�n.
    void seleccionarSiguiente();

    // Navega a la opci�n anterior.
    void seleccionarAnterior();

    // Agrega un objeto al men�.
    void AgregarObjeto(const SDL_Rect& srcRect, const SDL_Rect& destRect, const std::string& idTextura);

    // Establece el modo de visualizaci�n del men�.
    void darModo(ModoMenu nuevoModo);

    // Actualiza la opci�n actualmente seleccionada con un nuevo valor.
    void actualizarOpcionConVariable(int nuevaVariable);

    // Renderiza el men�.
    void dibujarMenu();

    // Devuelve el valor asociado a la opci�n actualmente seleccionada.
    int obtenerValorSeleccionado() const;

    // Activa o desactiva el resaltado para la opci�n seleccionada.
    void activarResaltado();
    void desactivarResaltado();

    // Nuevo m�todo: devuelve el modo actual del men�.
    ModoMenu obtenerModo() const;

private:
    std::vector<std::string> opciones;  // Lista de opciones.
    std::vector<int> valoresOpciones;   // Valores asociados a cada opci�n.
    int opcionSeleccionada;             // �ndice de la opci�n actual.
    std::vector<Objeto*> objetos;       // Objetos a renderizar junto al men�.
    ModoMenu modo;                      // Modo de visualizaci�n.

    // Miembros para el control de color.
    SDL_Color colorOriginal;            // Color base del texto.
    SDL_Color colorResaltado;           // Color cuando la opci�n est� resaltada.
    bool resaltadoActivo;               // Flag para activar/desactivar el resaltado.
};

#endif // MENUS_H
