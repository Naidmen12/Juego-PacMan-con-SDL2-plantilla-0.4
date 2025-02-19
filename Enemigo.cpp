#include "Enemigo.h"
#include "ManejarTextura.h"
#include <cstdlib>
#include <cmath>   // Para std::abs

// Constructor: Se almacena el ID y se inicializan las propiedades locales y la animación.
// Se utiliza el rectángulo fuente (srcRect) recibido para inicializar la animación, con un retardo de 10 frames.
Enemigo::Enemigo(std::string idTextura, SDL_Rect desRect, SDL_Rect srcRect, float velocidad)
    : Objeto(idTextura, desRect, srcRect),
    estado(Estado::Patrullando),
    idTexturaEnemigo(idTextura),
    animacion(idTextura, srcRect, 10),  // Inicializa la animación con retardoFrames = 10
    filaActualAnimacion(-1)             // Valor inicial para detectar el primer cambio de dirección
{
    posicion = Vector2D(static_cast<float>(desRect.x), static_cast<float>(desRect.y));
    Velocidad = Vector2D(velocidad, velocidad);

    // Inicializamos las propiedades locales a valores por defecto (completamente opaco y sin tinte)
    propiedadesLocales.r = 255;
    propiedadesLocales.g = 255;
    propiedadesLocales.b = 255;
    propiedadesLocales.alpha = 255;
}

void Enemigo::LogicaEnemigo(Mapa& mapa, Entidad& entidad, int gidBloqueador) {
    // Se asume que la posición del enemigo es el centro del sprite.
    Vector2D posEnemigo = this->ObtenerPosicion();
    Vector2D posJugador = entidad.ObtenerPosicion();
    float distancia = Vector2D::Distancia(posEnemigo, posJugador);

    const float RANGO_PERSECUCION = 160.0f;   // Ej: 5 celdas * 32 píxeles
    const float TOLERANCIA_WAYPOINT = 5.0f;     // Distancia para considerar alcanzado un waypoint

    // Vector que almacenará el movimiento aplicado en este ciclo, para actualizar la animación.
    Vector2D movimientoAnimacion(0, 0);

    if (distancia <= RANGO_PERSECUCION) {
        // --- MODO PERSECUCIÓN ---
        if (estado != Estado::Persiguiendo) {
            estado = Estado::Persiguiendo;
            rutaActual = Pathfinding::buscarCaminoAStar(mapa, posEnemigo, posJugador, gidBloqueador);
            indiceRuta = 0;
        }
        else {
            if (indiceRuta >= rutaActual.size()) {
                rutaActual = Pathfinding::buscarCaminoAStar(mapa, posEnemigo, posJugador, gidBloqueador);
                indiceRuta = 0;
            }
        }

        if (rutaActual.size() < 2) {
            Vector2D direccion = (posJugador - posEnemigo).Normalizado();
            if (!direccion.EsCero()) {
                Vector2D movimiento = direccion * Velocidad.x;
                posicion += movimiento;
                movimientoAnimacion = movimiento;
            }
        }
        else {
            if (indiceRuta < rutaActual.size()) {
                Vector2D waypoint = rutaActual[indiceRuta];
                Vector2D diff = waypoint - posEnemigo;
                if (diff.Magnitud() < TOLERANCIA_WAYPOINT) {
                    indiceRuta++;
                    if (indiceRuta >= rutaActual.size()) {
                        rutaActual = Pathfinding::buscarCaminoAStar(mapa, posEnemigo, posJugador, gidBloqueador);
                        indiceRuta = 0;
                    }
                }
                else {
                    Vector2D movimiento = diff.Normalizado() * Velocidad.x;
                    posicion += movimiento;
                    movimientoAnimacion = movimiento;
                }
            }
        }
        // Actualiza la posición del rectángulo destino (se asume centrado)
        destRect.x = static_cast<int>(posicion.x - destRect.w / 2);
        destRect.y = static_cast<int>(posicion.y - destRect.h / 2);
    }
    else {
        // --- MODO PATRULLA ---
        if (estado != Estado::Patrullando) {
            estado = Estado::Patrullando;
            rutaActual.clear();
            indiceRuta = 0;
        }
        if (rutaActual.empty() || indiceRuta >= rutaActual.size()) {
            const auto& matriz = mapa.ObtenerMatrizTiles();
            int filas = static_cast<int>(matriz.size());
            int columnas = static_cast<int>(matriz[0].size());
            int tileSize = matriz[0][0].destRect.w;  // Por ejemplo, 32 píxeles

            Vector2D destinoPatrulla;
            bool destinoValido = false;
            while (!destinoValido) {
                int celdaX = rand() % columnas;
                int celdaY = rand() % filas;
                if (matriz[celdaY][celdaX].gid != gidBloqueador) {
                    destinoPatrulla.x = celdaX * tileSize + static_cast<float>(tileSize) / 2.0f;
                    destinoPatrulla.y = celdaY * tileSize + static_cast<float>(tileSize) / 2.0f;
                    destinoValido = true;
                }
            }
            rutaActual = Pathfinding::buscarCaminoAStar(mapa, posEnemigo, destinoPatrulla, gidBloqueador);
            indiceRuta = 0;
        }
        if (!rutaActual.empty() && indiceRuta < rutaActual.size()) {
            Vector2D waypoint = rutaActual[indiceRuta];
            Vector2D diff = waypoint - posEnemigo;
            if (diff.Magnitud() < TOLERANCIA_WAYPOINT) {
                indiceRuta++;
            }
            else {
                Vector2D movimiento = diff.Normalizado() * Velocidad.x;
                posicion += movimiento;
                movimientoAnimacion = movimiento;
            }
            destRect.x = static_cast<int>(posicion.x - destRect.w / 2);
            destRect.y = static_cast<int>(posicion.y - destRect.h / 2);
        }
    }

    // ============================================
    // Actualización de la animación según la dirección del movimiento.
    // Se determinan las filas de animación:
    //   - 0: izquierda
    //   - 1: derecha
    //   - 2: arriba
    //   - 3: abajo
    // ============================================
    if (!movimientoAnimacion.EsCero()) {
        float absX = std::abs(movimientoAnimacion.x);
        float absY = std::abs(movimientoAnimacion.y);
        int filaAnimacion;
        if (absX > absY) {
            // Movimiento horizontal predominante
            if (movimientoAnimacion.x < 0)
                filaAnimacion = 0; // izquierda
            else
                filaAnimacion = 1; // derecha
        }
        else {
            // Movimiento vertical predominante
            if (movimientoAnimacion.y < 0)
                filaAnimacion = 2; // arriba
            else
                filaAnimacion = 3; // abajo
        }
        // Si la dirección (fila) cambió, reiniciamos la animación para evitar cambios bruscos.
        if (filaAnimacion != filaActualAnimacion) {
            filaActualAnimacion = filaAnimacion;
            animacion.reiniciar();
        }
        SDL_Rect nuevoFrame = animacion.actualizar(filaActualAnimacion);
        srcRect = nuevoFrame;
    }
    else {
        // Si no hay movimiento, se reinicia la animación (o se puede mantener el último frame).
        animacion.reiniciar();
    }

    // ====================================
    // Actualización de las propiedades locales (por ejemplo, el efecto de fading en modo patrulla)
    // ====================================
    if (estado == Estado::Patrullando) {
        const int pasoAlpha = 2;  // Velocidad de cambio
        if (fadingOut) {
            if (propiedadesLocales.alpha > pasoAlpha)
                propiedadesLocales.alpha -= pasoAlpha;
            else {
                propiedadesLocales.alpha = 0;
                fadingOut = false;
            }
        }
        else {
            if (propiedadesLocales.alpha < 255 - pasoAlpha)
                propiedadesLocales.alpha += pasoAlpha;
            else {
                propiedadesLocales.alpha = 255;
                fadingOut = true;
            }
        }
        // En modo patrulla, se utiliza color blanco
        propiedadesLocales.r = 255;
        propiedadesLocales.g = 255;
        propiedadesLocales.b = 255;
    }
    else {
        // En modo persecución se utiliza un tinte rojizo
        propiedadesLocales.alpha = 255;
        propiedadesLocales.r = 255;
        propiedadesLocales.g = 50;
        propiedadesLocales.b = 50;
    }
}

void Enemigo::Dibujar(SDL_Renderer* renderizador) {
    SDL_Texture* textura = ManejarTextura::Instancia()->ObtenerTextura(idTexturaEnemigo);
    if (textura) {
        SDL_SetTextureAlphaMod(textura, propiedadesLocales.alpha);
        SDL_SetTextureColorMod(textura, propiedadesLocales.r, propiedadesLocales.g, propiedadesLocales.b);
        SDL_RenderCopy(renderizador, textura, &srcRect, &destRect);
    }
}

void Enemigo::Actualizar() {
    // En este ejemplo se actualiza la posición según la velocidad.
    posicion += Velocidad;
    destRect.x = static_cast<int>(posicion.x);
    destRect.y = static_cast<int>(posicion.y);
}

void Enemigo::Limpiar() {
    Objeto::Limpiar();
}

void Enemigo::EstablecerEstado(Estado nuevoEstado) {
    estado = nuevoEstado;
}

Vector2D Enemigo::ObtenerPosicion() const {
    return posicion;
}

SDL_Rect Enemigo::ObtenerDestRect() const {
    return destRect;
}

Vector2D Enemigo::ObtenerVelocidad() const {
    return Velocidad;
}

void Enemigo::EstablecerVelocidad(float x, float y) {
    Velocidad.x = x;
    Velocidad.y = y;
}

void Enemigo::EstablecerPosicion(float x, float y) {
    posicion.x = x;
    posicion.y = y;
    // Se actualiza el rectángulo destino (se asume que destRect.w y destRect.h ya están establecidos).
    destRect.x = static_cast<int>(x - destRect.w);
    destRect.y = static_cast<int>(y - destRect.h);
}

/*#include "Enemigo.h"
#include "ManejarTextura.h"

// Constructor: Se almacena el ID y se inicializan las propiedades locales
Enemigo::Enemigo(std::string idTextura, SDL_Rect desRect, SDL_Rect srcRect, float velocidad)
    : Objeto(idTextura, desRect, srcRect),
    estado(Estado::Patrullando),
    idTexturaEnemigo(idTextura)
{
    posicion = Vector2D(static_cast<float>(desRect.x), static_cast<float>(desRect.y));
    Velocidad = Vector2D(velocidad, velocidad);

    // Inicializamos las propiedades locales a valores por defecto (completamente opaco y sin tinte)
    propiedadesLocales.r = 255;
    propiedadesLocales.g = 255;
    propiedadesLocales.b = 255;
    propiedadesLocales.alpha = 255;
}

void Enemigo::LogicaEnemigo(Mapa& mapa, Entidad& entidad, int gidBloqueador) {
    // Se asume que la posición del enemigo es el centro del sprite.
    Vector2D posEnemigo = this->ObtenerPosicion();
    Vector2D posJugador = entidad.ObtenerPosicion();
    float distancia = Vector2D::Distancia(posEnemigo, posJugador);

    const float RANGO_PERSECUCION = 160.0f;   // Ej: 5 celdas * 32 píxeles
    const float TOLERANCIA_WAYPOINT = 5.0f;     // Distancia para considerar alcanzado un waypoint

    if (distancia <= RANGO_PERSECUCION) {
        // --- MODO PERSECUCIÓN ---
        if (estado != Estado::Persiguiendo) {
            estado = Estado::Persiguiendo;
            rutaActual = Pathfinding::buscarCaminoAStar(mapa, posEnemigo, posJugador, gidBloqueador);
            indiceRuta = 0;
        }
        else {
            if (indiceRuta >= rutaActual.size()) {
                rutaActual = Pathfinding::buscarCaminoAStar(mapa, posEnemigo, posJugador, gidBloqueador);
                indiceRuta = 0;
            }
        }

        if (rutaActual.size() < 2) {
            Vector2D direccion = (posJugador - posEnemigo).Normalizado();
            if (!direccion.EsCero()) {
                Vector2D movimiento = direccion * Velocidad.x;
                posicion += movimiento;
            }
        }
        else {
            if (indiceRuta < rutaActual.size()) {
                Vector2D waypoint = rutaActual[indiceRuta];
                Vector2D diff = waypoint - posEnemigo;
                if (diff.Magnitud() < TOLERANCIA_WAYPOINT) {
                    indiceRuta++;
                    if (indiceRuta >= rutaActual.size()) {
                        rutaActual = Pathfinding::buscarCaminoAStar(mapa, posEnemigo, posJugador, gidBloqueador);
                        indiceRuta = 0;
                    }
                }
                else {
                    Vector2D movimiento = diff.Normalizado() * Velocidad.x;
                    posicion += movimiento;
                }
            }
        }
        // Actualiza la posición del rectángulo destino (se asume centrado)
        destRect.x = static_cast<int>(posicion.x - destRect.w / 2);
        destRect.y = static_cast<int>(posicion.y - destRect.h / 2);
    }
    else {
        // --- MODO PATRULLA ---
        if (estado != Estado::Patrullando) {
            estado = Estado::Patrullando;
            rutaActual.clear();
            indiceRuta = 0;
        }
        if (rutaActual.empty() || indiceRuta >= rutaActual.size()) {
            const auto& matriz = mapa.ObtenerMatrizTiles();
            int filas = static_cast<int>(matriz.size());
            int columnas = static_cast<int>(matriz[0].size());
            int tileSize = matriz[0][0].destRect.w;  // Por ejemplo, 32 píxeles

            Vector2D destinoPatrulla;
            bool destinoValido = false;
            while (!destinoValido) {
                int celdaX = rand() % columnas;
                int celdaY = rand() % filas;
                if (matriz[celdaY][celdaX].gid != gidBloqueador) {
                    destinoPatrulla.x = celdaX * tileSize + static_cast<float>(tileSize) / 2.0f;
                    destinoPatrulla.y = celdaY * tileSize + static_cast<float>(tileSize) / 2.0f;
                    destinoValido = true;
                }
            }
            rutaActual = Pathfinding::buscarCaminoAStar(mapa, posEnemigo, destinoPatrulla, gidBloqueador);
            indiceRuta = 0;
        }
        if (!rutaActual.empty() && indiceRuta < rutaActual.size()) {
            Vector2D waypoint = rutaActual[indiceRuta];
            Vector2D diff = waypoint - posEnemigo;
            if (diff.Magnitud() < TOLERANCIA_WAYPOINT) {
                indiceRuta++;
            }
            else {
                Vector2D movimiento = diff.Normalizado() * Velocidad.x;
                posicion += movimiento;
            }
            destRect.x = static_cast<int>(posicion.x - destRect.w / 2);
            destRect.y = static_cast<int>(posicion.y - destRect.h / 2);
        }
    }

    // ====================================
    // Actualización de las propiedades locales
    // ====================================
    if (estado == Estado::Patrullando) {
        // En modo patrulla se aplica el efecto de fading (transparencia pulsante)
        const int pasoAlpha = 2;  // Velocidad de cambio
        if (fadingOut) {
            if (propiedadesLocales.alpha > pasoAlpha)
                propiedadesLocales.alpha -= pasoAlpha;
            else {
                propiedadesLocales.alpha = 0;
                fadingOut = false;
            }
        }
        else {
            if (propiedadesLocales.alpha < 255 - pasoAlpha)
                propiedadesLocales.alpha += pasoAlpha;
            else {
                propiedadesLocales.alpha = 255;
                fadingOut = true;
            }
        }
        // En modo patrulla, se usa color blanco
        propiedadesLocales.r = 255;
        propiedadesLocales.g = 255;
        propiedadesLocales.b = 255;
    }
    else {
        // En modo persecución (Persiguiendo)
        propiedadesLocales.alpha = 255;
        // Se aplica un tinte rojizo (puedes ajustar los valores)
        propiedadesLocales.r = 255;
        propiedadesLocales.g = 50;
        propiedadesLocales.b = 50;
    }
}

void Enemigo::Dibujar(SDL_Renderer* renderizador) {
    // Se obtiene la textura compartida usando el ID almacenado
    SDL_Texture* textura = ManejarTextura::Instancia()->ObtenerTextura(idTexturaEnemigo);
    if (textura) {
        // Se aplican las propiedades locales (en lugar de las globales)
        SDL_SetTextureAlphaMod(textura, propiedadesLocales.alpha);
        SDL_SetTextureColorMod(textura, propiedadesLocales.r, propiedadesLocales.g, propiedadesLocales.b);
        SDL_RenderCopy(renderizador, textura, &srcRect, &destRect);
    }
}

void Enemigo::Actualizar() {
    // Esta función puede actualizar otros aspectos; aquí se muestra un ejemplo de actualización de posición.
    posicion += Velocidad;
    destRect.x = static_cast<int>(posicion.x);
    destRect.y = static_cast<int>(posicion.y);
}

void Enemigo::Limpiar() {
    Objeto::Limpiar();
}

void Enemigo::EstablecerEstado(Estado nuevoEstado) {
    estado = nuevoEstado;
}

Vector2D Enemigo::ObtenerPosicion() const {
    return posicion;
}

SDL_Rect Enemigo::ObtenerDestRect() const {
    return destRect;
}

Vector2D Enemigo::ObtenerVelocidad() const {
    return Velocidad;
}

void Enemigo::EstablecerVelocidad(float x, float y) {
    Velocidad.x = x;
    Velocidad.y = y;
}

void Enemigo::EstablecerPosicion(float x, float y) {
    posicion.x = x;
    posicion.y = y;
    // Se actualiza el rectángulo destino. Se asume que destRect.w y destRect.h ya están establecidos.
    destRect.x = static_cast<int>(x - destRect.w);
    destRect.y = static_cast<int>(y - destRect.h);
}*/