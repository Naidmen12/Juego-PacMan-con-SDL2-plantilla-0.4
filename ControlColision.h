#ifndef CONTROL_COLISION_H
#define CONTROL_COLISION_H

#include <iostream>
#include "Colision.h"
#include "Enemigo.h"
#include "Entidad.h"
#include "Mapa.h"

class ControlColision : public Colision {
public:
    ControlColision();
    LadoColision ComprobarColision(Entidad& entidad, const Mapa& mapa, int gid) override;
    void ColisionEQ(Entidad* entidad, Mapa* mapa, int gid);
    int ObtenerContador() const;  // Retorna el valor del contador

    // Nueva función: retorna true si la entidad colisiona con el enemigo.
    bool ColisionEnemigo(Entidad& entidad, Enemigo& enemigo);

private:
    int Contador;  // Contador de colisiones
};

#endif // CONTROL_COLISION_H
