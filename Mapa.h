#ifndef MAPA_H
#define MAPA_H

#include <string>
#include <vector>
#include <SDL.h>
#include "tinyxml2.h"
#include "ManejarTextura.h"

// Declaración adelantada de SDL_Texture
struct SDL_Texture;

struct TileData {
    int gid;
    SDL_Rect srcRect;
    SDL_Rect destRect;
};

class Mapa {
public:
    Mapa(SDL_Renderer* renderer);
    ~Mapa();

    bool CargarMapa(const std::string& archivoMapa, const std::string& archivoTextura);
    bool CargarTileset(const std::string& archivoTileset);
    void RenderizarMapa();
    int RetornarContado();
    std::vector<std::vector<TileData>>& ObtenerMatrizTiles();
    const std::vector<std::vector<TileData>>& ObtenerMatrizTiles() const;

    int anchoTile;
    int altoTile;
    int anchoMapa;
    int altoMapa;
    int imagenAncho;
    int imagenAlto;

private:
    SDL_Renderer* renderer;
    SDL_Texture* texturaMapa;
    std::vector<std::vector<TileData>> matrizTiles;
    int Contador;

    void CargarTiles(tinyxml2::XMLElement* elemento);
    void CargarColisiones(tinyxml2::XMLElement* tileset);
};

#endif // MAPA_H