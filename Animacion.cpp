#include "Animacion.h"
#include "ManejarTextura.h"

Animacion::Animacion(const std::string& idTextura, SDL_Rect rectFrame, int retardoFrames)
    : idTextura(idTextura), rectFrameActual(rectFrame), retardoFrames(retardoFrames),
    contadorFrames(0){
    textura = ManejarTextura::Instancia()->ObtenerTextura(idTextura);
    if (!textura) {
        SDL_Log("Error: No se pudo obtener la textura con ID %s", idTextura.c_str());
    }
    obtenerCaracteristicasTextura();
    // Se inicializa rectFrameActual.y en 0 por defecto
    rectFrameActual.y = 0;
}

void Animacion::obtenerCaracteristicasTextura() {
    int anchoTextura = 0, altoTextura = 0;
    if (SDL_QueryTexture(textura, nullptr, nullptr, &anchoTextura, &altoTextura) != 0) {
        SDL_Log("Error al consultar la textura: %s", SDL_GetError());
    }
    columnas = anchoTextura / rectFrameActual.w;
    filas = altoTextura / rectFrameActual.h;
}

SDL_Rect Animacion::actualizar(int filaFija) {
    contadorFrames++;
    if (contadorFrames < retardoFrames)
        return rectFrameActual;

    contadorFrames = 0;

    // Validar que la fila recibida esté dentro del rango permitido
    if (filaFija >= filas || filaFija < 0) {
        SDL_Log("Fila fija (%d) fuera de rango. Se establecerá en 0.", filaFija);
        filaFija = 0;
    }

    int columnaActual = rectFrameActual.x / rectFrameActual.w;
    if (columnaActual < columnas - 1)
        columnaActual++;
    else
        columnaActual = 0;

    rectFrameActual.x = columnaActual * rectFrameActual.w;
    rectFrameActual.y = filaFija * rectFrameActual.h;

    return rectFrameActual;
}

void Animacion::setRetardoFrames(int nuevoRetardo) {
    retardoFrames = nuevoRetardo;
}

int Animacion::getRetardoFrames() const {
    return retardoFrames;
}

void Animacion::reiniciar() {
    rectFrameActual.x = 0;
    // Al no tener almacenado filaFija, se resetea a 0; 
    // en caso de que se requiera otro comportamiento, se podría modificar este método.
    rectFrameActual.y = 0;
    contadorFrames = 0;
}

SDL_Texture* Animacion::obtenerTextura() const {
    return textura;
}