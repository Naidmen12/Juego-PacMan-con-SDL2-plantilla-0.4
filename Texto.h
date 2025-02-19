#ifndef TEXTO_H
#define TEXTO_H

#include <string>
#include <map>
#include <SDL_ttf.h>
#include <iostream>

class Texto {
private:
    // Instancia singleton
    static Texto* instancia;
    // Mapa para almacenar las fuentes (TTF_Font*) identificadas por una etiqueta
    std::map<std::string, TTF_Font*> fuentes;

    // Constructor privado para implementar el singleton
    Texto() {}

public:
    // Retorna la instancia singleton
    static Texto* Instancia();

    // Carga una fuente desde un archivo .ttf con el tamaño especificado y la asocia al ID dado.
    // Retorna true si se carga correctamente, o false en caso de error.
    bool Cargar(const std::string& archivo, const std::string& id, int fontSize);

    // Obtiene la fuente asociada al ID proporcionado.
    TTF_Font* ObtenerFuente(const std::string& id);

    // Libera todas las fuentes cargadas y limpia el mapa.
    void Limpiar();

    // Destructor: se encarga de liberar las fuentes.
    ~Texto();
};

#endif // TEXTO_H

