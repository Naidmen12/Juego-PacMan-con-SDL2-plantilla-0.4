#include "ManejoTexto.h"
#include <iostream>
#include <sstream>

ManejoTexto::ManejoTexto(SDL_Renderer* renderer, const std::string& fontLabel, const SDL_Rect& rect, SDL_Color color)
    : renderer(renderer), rect(rect), color(color), fontLabel(fontLabel),
    currentStaticMessage(""), currentVariable(-1),
    staticTexture(nullptr), variableTexture(nullptr)
{
}

ManejoTexto::~ManejoTexto() {
    if (staticTexture) {
        SDL_DestroyTexture(staticTexture);
    }
    if (variableTexture) {
        SDL_DestroyTexture(variableTexture);
    }
}

void ManejoTexto::Mensaje(const std::string& mensaje, int variable) {
    // Actualizamos la parte fija si ha cambiado
    if (mensaje != currentStaticMessage) {
        currentStaticMessage = mensaje;
        if (staticTexture) {
            SDL_DestroyTexture(staticTexture);
            staticTexture = nullptr;
        }
        TTF_Font* font = Texto::Instancia()->ObtenerFuente(fontLabel);
        if (!font) {
            std::cerr << "Error: Fuente no encontrada: " << fontLabel << std::endl;
            return;
        }
        SDL_Surface* surface = TTF_RenderText_Blended(font, mensaje.c_str(), color);
        if (!surface) {
            std::cerr << "Error al renderizar texto estático: " << TTF_GetError() << std::endl;
            return;
        }
        staticTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!staticTexture) {
            std::cerr << "Error al crear textura estática: " << SDL_GetError() << std::endl;
        }
    }
    // Actualizamos la parte variable si es diferente
    if (variable != currentVariable) {
        currentVariable = variable;
        if (variableTexture) {
            SDL_DestroyTexture(variableTexture);
            variableTexture = nullptr;
        }
        // Si variable es menor que 0, no mostramos la parte variable
        if (variable < 0)
            return;
        // Convertimos el número a cadena (sin ancho fijo para que se desplace naturalmente)
        std::ostringstream oss;
        oss << variable;
        std::string varStr = oss.str();
        TTF_Font* font = Texto::Instancia()->ObtenerFuente(fontLabel);
        if (!font) {
            std::cerr << "Error: Fuente no encontrada: " << fontLabel << std::endl;
            return;
        }
        SDL_Surface* surfaceVar = TTF_RenderText_Blended(font, varStr.c_str(), color);
        if (!surfaceVar) {
            std::cerr << "Error al renderizar texto variable: " << TTF_GetError() << std::endl;
            return;
        }
        variableTexture = SDL_CreateTextureFromSurface(renderer, surfaceVar);
        SDL_FreeSurface(surfaceVar);
        if (!variableTexture) {
            std::cerr << "Error al crear textura variable: " << SDL_GetError() << std::endl;
        }
    }
}

void ManejoTexto::DibujarMensaje() {
    // Posición base donde se dibuja el mensaje
    int x = rect.x;
    int y = rect.y;
    // Dibujamos la parte fija
    if (staticTexture) {
        int w, h;
        SDL_QueryTexture(staticTexture, nullptr, nullptr, &w, &h);
        SDL_Rect destStatic = { x, rect.y + (rect.h - h) / 2, w, h };
        SDL_RenderCopy(renderer, staticTexture, nullptr, &destStatic);
        // Si existe la parte variable, se dibuja a la derecha de la parte fija
        if (variableTexture) {
            int wVar, hVar;
            SDL_QueryTexture(variableTexture, nullptr, nullptr, &wVar, &hVar);
            SDL_Rect destVar = { x + w + 5, rect.y + (rect.h - hVar) / 2, wVar, hVar };
            SDL_RenderCopy(renderer, variableTexture, nullptr, &destVar);
        }
    }
}