#include "Menus.h"
#include <iostream>
#include "Texto.h"      // Para obtener la fuente
#include <SDL_ttf.h>    // Para renderizar texto

Menus::Menus(SDL_Renderer* renderer, const std::string& fontLabel, const SDL_Rect& rect, SDL_Color color)
    : ManejoTexto(renderer, fontLabel, rect, color),
    opcionSeleccionada(0),
    modo(ModoMenu::UNICO),
    colorOriginal(color),
    resaltadoActivo(false)
{
    // Definimos un color de resaltado (por ejemplo, rojo).
    colorResaltado = { 255, 0, 0, 255 };
}

Menus::~Menus() {
    for (Objeto* obj : objetos) {
        delete obj;
    }
    objetos.clear();
}

void Menus::agregarOpcion(const std::string& opcion, int variable) {
    opciones.push_back(opcion);
    valoresOpciones.push_back(variable);

    // Si es la primera opción y estamos en modo UNICO, se actualiza el mensaje.
    if (opciones.size() == 1 && modo == ModoMenu::UNICO) {
        Mensaje(opcion, variable);
    }
}

void Menus::actualizarOpcionConVariable(int nuevaVariable) {
    if (!opciones.empty() && modo == ModoMenu::UNICO) {
        Mensaje(opciones[opcionSeleccionada], nuevaVariable);
    }
}

void Menus::seleccionarSiguiente() {
    if (opciones.empty()) return;
    opcionSeleccionada = (opcionSeleccionada + 1) % opciones.size();
    if (modo == ModoMenu::UNICO)
        Mensaje(opciones[opcionSeleccionada], -1);
}

void Menus::seleccionarAnterior() {
    if (opciones.empty()) return;
    opcionSeleccionada = (opcionSeleccionada - 1 + opciones.size()) % opciones.size();
    if (modo == ModoMenu::UNICO)
        Mensaje(opciones[opcionSeleccionada], -1);
}

void Menus::AgregarObjeto(const SDL_Rect& srcRect, const SDL_Rect& destRect, const std::string& idTextura) {
    Objeto* nuevoObjeto = new Objeto(idTextura, destRect, srcRect);
    objetos.push_back(nuevoObjeto);
}

void Menus::darModo(ModoMenu nuevoModo) {
    modo = nuevoModo;
    if (modo == ModoMenu::UNICO && !opciones.empty()) {
        Mensaje(opciones[opcionSeleccionada], -1);
    }
}

void Menus::dibujarMenu() {
    // Dibujar objetos asociados.
    for (Objeto* obj : objetos) {
        obj->Dibujar(renderer);
    }

    if (modo == ModoMenu::UNICO) {
        DibujarMensaje();
    }
    else if (modo == ModoMenu::LISTA) {
        TTF_Font* font = Texto::Instancia()->ObtenerFuente(fontLabel);
        if (!font) {
            std::cerr << "Error: No se encontró la fuente \"" << fontLabel << "\"." << std::endl;
        }
        else {
            for (size_t i = 0; i < opciones.size(); ++i) {
                // Si es la opción seleccionada y el resaltado está activo, usar colorResaltado.
                SDL_Color textoColor = colorOriginal;
                if (i == static_cast<size_t>(opcionSeleccionada) && resaltadoActivo) {
                    textoColor = colorResaltado;
                }
                SDL_Surface* surface = TTF_RenderText_Blended(font, opciones[i].c_str(), textoColor);
                if (!surface) {
                    std::cerr << "Error al renderizar el texto: " << TTF_GetError() << std::endl;
                    continue;
                }
                SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);
                if (!tempTexture) {
                    std::cerr << "Error al crear la textura: " << SDL_GetError() << std::endl;
                    continue;
                }
                SDL_Rect dest = rect;
                dest.y = rect.y + static_cast<int>(i) * (rect.h + 16);
                SDL_RenderCopy(renderer, tempTexture, NULL, &dest);
                SDL_DestroyTexture(tempTexture);
            }
        }
    }
}

int Menus::obtenerValorSeleccionado() const {
    if (opciones.empty()) return -1;
    return valoresOpciones[opcionSeleccionada];
}

void Menus::activarResaltado() {
    resaltadoActivo = true;
}

void Menus::desactivarResaltado() {
    resaltadoActivo = false;
}

ModoMenu Menus::obtenerModo() const {
    return modo;
}