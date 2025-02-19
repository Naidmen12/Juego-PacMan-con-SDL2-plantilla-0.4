#include "Mapa.h"
#include <iostream>
#include <sstream>

Mapa::Mapa(SDL_Renderer* renderer)
    : renderer(renderer),
    texturaMapa(nullptr),
    anchoTile(0),
    altoTile(0),
    anchoMapa(0),
    altoMapa(0),
    imagenAncho(0),
    imagenAlto(0),
    Contador(0){
}

Mapa::~Mapa() {
    if (texturaMapa) {
        SDL_DestroyTexture(texturaMapa);
    }
}

bool Mapa::CargarMapa(const std::string& archivoMapa, const std::string& archivoTextura) {
    std::cout << "Cargando el archivo del mapa: " << archivoMapa << std::endl;
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.LoadFile(archivoMapa.c_str());
    if (error != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error al cargar el archivo del mapa: " << archivoMapa << std::endl;
        std::cerr << "tinyxml2 Error: " << doc.ErrorStr() << std::endl;
        return false;
    }

    tinyxml2::XMLElement* mapa = doc.FirstChildElement("map");
    if (!mapa) {
        std::cerr << "No se encontró el elemento 'map' en el archivo del mapa." << std::endl;
        return false;
    }

    std::cout << "Cargando atributos del mapa" << std::endl;
    mapa->QueryIntAttribute("width", &anchoMapa);
    mapa->QueryIntAttribute("height", &altoMapa);
    mapa->QueryIntAttribute("tilewidth", &anchoTile);
    mapa->QueryIntAttribute("tileheight", &altoTile);

    tinyxml2::XMLElement* tileset = mapa->FirstChildElement("tileset");
    if (!tileset) {
        std::cerr << "No se encontró el elemento 'tileset' en el archivo del mapa." << std::endl;
        return false;
    }

    const char* source = tileset->Attribute("source");
    if (!source) {
        std::cerr << "No se encontró el atributo 'source' en el elemento 'tileset'." << std::endl;
        return false;
    }

    // Construye la ruta completa al archivo del tileset (por ejemplo, en la carpeta "Contenido")
    std::string archivoTileset = "Contenido/" + std::string(source);
    std::cout << "Cargando el archivo del tileset: " << archivoTileset << std::endl;
    if (!CargarTileset(archivoTileset)) {
        return false;
    }

    std::cout << "Cargando la textura del mapa: " << archivoTextura << std::endl;
    texturaMapa = ManejarTextura::Instancia()->Cargar(archivoTextura, "texturaMapa", renderer);
    if (!texturaMapa) {
        std::cerr << "Error al cargar la textura del mapa: " << archivoTextura << std::endl;
        return false;
    }

    tinyxml2::XMLElement* capa = mapa->FirstChildElement("layer");
    if (!capa) {
        std::cerr << "No se encontró el elemento 'layer' en el archivo del mapa." << std::endl;
        return false;
    }
    std::cout << "Cargando tiles del mapa" << std::endl;
    CargarTiles(capa->FirstChildElement("data"));

    return true;
}

bool Mapa::CargarTileset(const std::string& archivoTileset) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.LoadFile(archivoTileset.c_str());
    if (error != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error al cargar el archivo del tileset: " << archivoTileset << std::endl;
        std::cerr << "tinyxml2 Error: " << doc.ErrorStr() << std::endl;
        return false;
    }

    tinyxml2::XMLElement* tileset = doc.FirstChildElement("tileset");
    if (!tileset) {
        std::cerr << "No se encontró el elemento 'tileset' en el archivo del tileset." << std::endl;
        return false;
    }

    tinyxml2::XMLElement* imagen = tileset->FirstChildElement("image");
    if (!imagen) {
        std::cerr << "No se encontró el elemento 'image' en el archivo del tileset." << std::endl;
        return false;
    }

    std::cout << "Cargando atributos de la imagen del tileset" << std::endl;
    imagen->QueryIntAttribute("width", &imagenAncho);
    imagen->QueryIntAttribute("height", &imagenAlto);

    std::cout << "Cargando colisiones del tileset" << std::endl;
    CargarColisiones(tileset);

    return true;
}

void Mapa::CargarColisiones(tinyxml2::XMLElement* tileset) {
    for (tinyxml2::XMLElement* tile = tileset->FirstChildElement("tile"); tile != nullptr; tile = tile->NextSiblingElement("tile")) {
        int id;
        tile->QueryIntAttribute("id", &id);

        tinyxml2::XMLElement* objectGroup = tile->FirstChildElement("objectgroup");
        if (objectGroup) {
            tinyxml2::XMLElement* object = objectGroup->FirstChildElement("object");
            if (object) {
                SDL_Rect colision;
                float x, y, width, height;
                object->QueryFloatAttribute("x", &x);
                object->QueryFloatAttribute("y", &y);
                object->QueryFloatAttribute("width", &width);
                object->QueryFloatAttribute("height", &height);
                colision.x = static_cast<int>(x);
                colision.y = static_cast<int>(y);
                colision.w = static_cast<int>(width);
                colision.h = static_cast<int>(height);
                std::cout << "Colision cargada para tile id: " << id << std::endl;
            }
        }
    }
}

void Mapa::CargarTiles(tinyxml2::XMLElement* elemento) {
    if (!elemento) {
        std::cerr << "No se encontró el elemento 'data' en el archivo del mapa." << std::endl;
        return;
    }
    matrizTiles.resize(altoMapa, std::vector<TileData>(anchoMapa));
    int i = 0, j = 0;
    const char* data = elemento->GetText();
    std::istringstream dataStream(data);
    std::string line;
    while (std::getline(dataStream, line, ',')) {
        int gid = std::stoi(line);
        if (gid == 1) {
            Contador++;
        }
        // Calcular los rectángulos srcRect y destRect
        SDL_Rect srcRect, destRect;
        if (gid != 0) {
            int adjustedGid = gid - 1; // Ajustar GID para índice de textura
            srcRect.w = anchoTile;
            srcRect.h = altoTile;
            srcRect.x = (adjustedGid % (imagenAncho / anchoTile)) * anchoTile;
            srcRect.y = (adjustedGid / (imagenAncho / anchoTile)) * altoTile;
        }
        else {
            srcRect = { 0, 0, 0, 0 };
        }
        destRect.w = anchoTile;
        destRect.h = altoTile;
        destRect.x = j * anchoTile;
        destRect.y = i * altoTile;

        matrizTiles[i][j] = { gid, srcRect, destRect };

        if (++j >= anchoMapa) {
            j = 0;
            ++i;
        }
    }
}

void Mapa::RenderizarMapa() {
    for (const auto& fila : matrizTiles) {
        for (const auto& tileData : fila) {
            if (tileData.gid == 0) continue;
            SDL_RenderCopy(renderer, texturaMapa, &tileData.srcRect, &tileData.destRect);
        }
    }
}

int Mapa::RetornarContado(){
    return Contador;
}

std::vector<std::vector<TileData>>& Mapa::ObtenerMatrizTiles() {
    return matrizTiles;
}

const std::vector<std::vector<TileData>>& Mapa::ObtenerMatrizTiles() const {
    return matrizTiles;
}