#ifndef COLISION_H
#define COLISION_H

#include <vector>
#include "Entidad.h"
#include "Mapa.h"

enum class LadoColision {
    Ninguno,
    Izquierda,
    Derecha,
    Arriba,
    Abajo
};

class Colision {
public:
    virtual ~Colision() = default; // Destructor virtual para evitar fugas de memoria en clases derivadas
    virtual LadoColision ComprobarColision(Entidad& entidad, const Mapa& mapa, int gid) = 0; // M�todo puro con el par�metro gid
};

#endif // COLISION_H
