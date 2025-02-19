#ifndef MANEJO_TEXTO_H
#define MANEJO_TEXTO_H

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "Texto.h"  // Clase que gestiona la carga de fuentes

class ManejoTexto {
protected:
    SDL_Renderer* renderer;
    SDL_Rect rect;              // Área donde se dibujará el mensaje (fija para la parte estática)
    SDL_Color color;
    std::string fontLabel;      // Etiqueta de la fuente a utilizar (debe estar cargada en Texto)

    // Almacenamos separadamente la parte fija y la parte variable
    std::string currentStaticMessage; // Mensaje fijo actual
    int currentVariable;              // Valor variable actual

    SDL_Texture* staticTexture;   // Textura para la parte fija
    SDL_Texture* variableTexture; // Textura para la parte variable

public:
    // Constructor: recibe el renderer, la etiqueta de la fuente, el rectángulo (posición y dimensiones) y el color del mensaje.
    ManejoTexto(SDL_Renderer* renderer, const std::string& fontLabel, const SDL_Rect& rect, SDL_Color color);

    // Destructor virtual: permite la correcta destrucción de objetos derivados.
    virtual ~ManejoTexto();

    // Actualiza el mensaje a renderizar.
    // Separamos la parte fija (mensaje) y la parte variable (variable).
    // Si 'variable' es menor que 0, solo se renderiza la parte fija.
    virtual void Mensaje(const std::string& mensaje, int variable);

    // Renderiza el mensaje (si existe) en la posición y tamaño definidos.
    // Se dibuja primero la parte fija y, si existe, la parte variable a la derecha.
    virtual void DibujarMensaje();
};

#endif // MANEJO_TEXTO_H