#include "Texto.h"

Texto* Texto::instancia = nullptr;

Texto* Texto::Instancia() {
    if (instancia == nullptr) {
        instancia = new Texto();
    }
    return instancia;
}

bool Texto::Cargar(const std::string& archivo, const std::string& id, int fontSize) {
    TTF_Font* font = TTF_OpenFont(archivo.c_str(), fontSize);
    if (!font) {
        std::cerr << "Error al cargar la fuente (" << archivo << "): " << TTF_GetError() << std::endl;
        return false;
    }
    fuentes[id] = font;
    return true;
}

TTF_Font* Texto::ObtenerFuente(const std::string& id) {
    if (fuentes.find(id) != fuentes.end()) {
        return fuentes[id];
    }
    std::cerr << "Fuente con ID " << id << " no encontrada." << std::endl;
    return nullptr;
}

void Texto::Limpiar() {
    for (auto& par : fuentes) {
        TTF_CloseFont(par.second);
    }
    fuentes.clear();
}

Texto::~Texto() {
    Limpiar();
}
