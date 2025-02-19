#ifndef MENUS_H
#define MENUS_H

#include "ManejoTexto.h"
#include "Objeto.h"
#include <vector>
#include <string>
#include <SDL.h>

// Enumerador para los modos de visualización del menú
enum class ModoMenu {
    UNICO, // Se muestra solo la opción seleccionada (navegación con IZQ/DER)
    LISTA  // Se muestra la lista completa (navegación con ARRIBA/ABAJO)
};

class Menus : public ManejoTexto {
public:
    // Constructor: se pasa el renderer, la etiqueta de la fuente, el rectángulo base y el color.
    Menus(SDL_Renderer* renderer, const std::string& fontLabel, const SDL_Rect& rect, SDL_Color color);

    // Destructor
    ~Menus();

    // Agrega una opción al menú.
    // El parámetro 'variable' es opcional (por defecto -1).
    void agregarOpcion(const std::string& opcion, int variable = -1);

    // Navega a la siguiente opción.
    void seleccionarSiguiente();

    // Navega a la opción anterior.
    void seleccionarAnterior();

    // Agrega un objeto al menú.
    void AgregarObjeto(const SDL_Rect& srcRect, const SDL_Rect& destRect, const std::string& idTextura);

    // Establece el modo de visualización del menú.
    void darModo(ModoMenu nuevoModo);

    // Actualiza la opción actualmente seleccionada con un nuevo valor.
    void actualizarOpcionConVariable(int nuevaVariable);

    // Renderiza el menú.
    void dibujarMenu();

    // Devuelve el valor asociado a la opción actualmente seleccionada.
    int obtenerValorSeleccionado() const;

    // Activa o desactiva el resaltado para la opción seleccionada.
    void activarResaltado();
    void desactivarResaltado();

    // Nuevo método: devuelve el modo actual del menú.
    ModoMenu obtenerModo() const;

private:
    std::vector<std::string> opciones;  // Lista de opciones.
    std::vector<int> valoresOpciones;   // Valores asociados a cada opción.
    int opcionSeleccionada;             // Índice de la opción actual.
    std::vector<Objeto*> objetos;       // Objetos a renderizar junto al menú.
    ModoMenu modo;                      // Modo de visualización.

    // Miembros para el control de color.
    SDL_Color colorOriginal;            // Color base del texto.
    SDL_Color colorResaltado;           // Color cuando la opción está resaltada.
    bool resaltadoActivo;               // Flag para activar/desactivar el resaltado.
};

#endif // MENUS_H
