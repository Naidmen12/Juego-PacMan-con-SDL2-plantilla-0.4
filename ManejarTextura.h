#ifndef MANEJARTEXTURA_H
#define MANEJARTEXTURA_H

#include <string>
#include <map>
#include <SDL.h>
#include <SDL_image.h>

struct PropiedadesTextura {
    Uint8 r = 255;
    Uint8 g = 255;
    Uint8 b = 255;
    Uint8 alpha = 255;
};

class ManejarTextura {
public:
    static ManejarTextura* Instancia() {
        if (instancia == nullptr) {
            instancia = new ManejarTextura();
        }
        return instancia;
    }

    SDL_Texture* Cargar(std::string archivo, std::string id, SDL_Renderer* renderizador);
    SDL_Texture* ObtenerTextura(std::string id);
    PropiedadesTextura* ObtenerPropiedades(std::string id);
    void Limpiar();

private:
    ManejarTextura() {}
    static ManejarTextura* instancia;
    std::map<std::string, SDL_Texture*> texturas;
    std::map<std::string, PropiedadesTextura> propiedades;
};

#endif // MANEJARTEXTURA_H

/*#ifndef MANEJARTEXTURA_H
#define MANEJARTEXTURA_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <map>
#include <iostream>

class ManejarTextura {
public:
    static ManejarTextura* Instancia() {
        if (instancia == nullptr) {
            instancia = new ManejarTextura();
        }
        return instancia;
    }

    bool Cargar(std::string archivo, std::string id, SDL_Renderer* renderizador);
    SDL_Texture* ObtenerTextura(std::string id);
    void Limpiar();

private:
    ManejarTextura() {}
    static ManejarTextura* instancia;
    std::map<std::string, SDL_Texture*> texturas;
};

#endif // MANEJARTEXTURA_H*/

