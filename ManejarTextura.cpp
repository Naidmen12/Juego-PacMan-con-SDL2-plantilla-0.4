#include "ManejarTextura.h"
#include <iostream>

ManejarTextura* ManejarTextura::instancia = nullptr;

SDL_Texture* ManejarTextura::Cargar(std::string archivo, std::string id, SDL_Renderer* renderizador) {
    SDL_Surface* superficie = IMG_Load(archivo.c_str());
    if (superficie == nullptr) {
        std::cerr << "Error al cargar la superficie: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* textura = SDL_CreateTextureFromSurface(renderizador, superficie);
    SDL_FreeSurface(superficie);

    if (textura == nullptr) {
        std::cerr << "Error al crear la textura: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    texturas[id] = textura;
    propiedades[id] = PropiedadesTextura();
    return textura;
}

SDL_Texture* ManejarTextura::ObtenerTextura(std::string id) {
    if (texturas.find(id) != texturas.end()) {
        return texturas[id];
    }
    std::cerr << "Textura con ID " << id << " no encontrada." << std::endl;
    return nullptr;
}

PropiedadesTextura* ManejarTextura::ObtenerPropiedades(std::string id) {
    if (propiedades.find(id) != propiedades.end()) {
        return &propiedades[id];
    }
    std::cerr << "No se pueden obtener propiedades de la textura con ID " << id << " porque no existe." << std::endl;
    return nullptr;
}

void ManejarTextura::Limpiar() {
    for (auto& textura : texturas) {
        SDL_DestroyTexture(textura.second);
    }
    texturas.clear();
    propiedades.clear();
}

/*#include "ManejarTextura.h"

ManejarTextura* ManejarTextura::instancia = nullptr;

bool ManejarTextura::Cargar(std::string archivo, std::string id, SDL_Renderer* renderizador) {
    SDL_Surface* superficie = IMG_Load(archivo.c_str());
    if (superficie == nullptr) {
        std::cerr << "Error al cargar la superficie: " << IMG_GetError() << std::endl;
        return false;
    }

    SDL_Texture* textura = SDL_CreateTextureFromSurface(renderizador, superficie);
    SDL_FreeSurface(superficie);

    if (textura == nullptr) {
        std::cerr << "Error al crear la textura: " << SDL_GetError() << std::endl;
        return false;
    }

    texturas[id] = textura;
    return true;
}

SDL_Texture* ManejarTextura::ObtenerTextura(std::string id) {
    if (texturas.find(id) != texturas.end()) {
        return texturas[id];
    }
    std::cerr << "Textura con ID " << id << " no encontrada." << std::endl;
    return nullptr;
}

void ManejarTextura::Limpiar() {
    for (auto& textura : texturas) {
        SDL_DestroyTexture(textura.second);
    }
    texturas.clear();
}*/
