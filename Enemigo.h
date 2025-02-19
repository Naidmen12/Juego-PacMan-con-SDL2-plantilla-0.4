#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <string>
#include <cstdlib>
#include <vector>
#include "Entidad.h"
#include "Mapa.h"
#include "Pathfinding.h"  // Se asume que aquí se define buscarCaminoAStar
#include "Animacion.h"    // Nuevo: incluir la clase de Animacion

class Enemigo : public Objeto {
public:
    enum class Estado { Patrullando, Persiguiendo };

    // El constructor recibe el ID de la textura
    Enemigo(std::string idTextura, SDL_Rect desRect, SDL_Rect srcRect, float velocidad);

    void LogicaEnemigo(Mapa& mapa, Entidad& entidad, int gidBloqueador);
    void Dibujar(SDL_Renderer* renderizador) override;
    void Actualizar();
    void Limpiar();
    void EstablecerEstado(Estado nuevoEstado);
    Vector2D ObtenerPosicion() const;
    SDL_Rect ObtenerDestRect() const;
    Vector2D ObtenerVelocidad() const;
    void EstablecerVelocidad(float x, float y);
    void EstablecerPosicion(float x, float y);

private:
    Estado estado;
    Vector2D posicion;
    Vector2D Velocidad;  // Velocidad en píxeles por frame
    std::vector<Vector2D> rutaActual;
    size_t indiceRuta = 0;

    // ID de la textura compartida
    std::string idTexturaEnemigo;

    // Propiedades locales para el renderizado (copia independiente)
    PropiedadesTextura propiedadesLocales;

    // Variable para controlar el efecto de fading (modo patrulla)
    bool fadingOut = true;

    // Nuevo: objeto de animación (se utiliza el srcRect inicial y un retardo por defecto, por ejemplo 10)
    Animacion animacion;
    int filaActualAnimacion; // Nueva variable para almacenar la fila actual de animación
};

#endif // ENEMIGO_H

/*#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <string>
#include <cstdlib>
#include <vector>
#include "Entidad.h"
#include "Mapa.h"
#include "Pathfinding.h"  // Se asume que aquí se define buscarCaminoAStar

class Enemigo : public Objeto {
public:
    enum class Estado { Patrullando, Persiguiendo };

    // El constructor recibe el ID de la textura
    Enemigo(std::string idTextura, SDL_Rect desRect, SDL_Rect srcRect, float velocidad);

    void LogicaEnemigo(Mapa& mapa, Entidad& entidad, int gidBloqueador);
    void Dibujar(SDL_Renderer* renderizador) override;
    void Actualizar();
    void Limpiar();
    void EstablecerEstado(Estado nuevoEstado);
    Vector2D ObtenerPosicion() const;
    SDL_Rect ObtenerDestRect() const;
    Vector2D ObtenerVelocidad() const;
    void EstablecerVelocidad(float x, float y);
    void EstablecerPosicion(float x, float y);

private:
    Estado estado;
    Vector2D posicion;
    Vector2D Velocidad;  // Velocidad en píxeles por frame
    std::vector<Vector2D> rutaActual;
    size_t indiceRuta = 0;

    // ID de la textura compartida
    std::string idTexturaEnemigo;

    // Propiedades locales para el renderizado (copia independiente)
    PropiedadesTextura propiedadesLocales;

    // Variable para controlar el efecto de fading (modo patrulla)
    bool fadingOut = true;
};

#endif // ENEMIGO_H*/