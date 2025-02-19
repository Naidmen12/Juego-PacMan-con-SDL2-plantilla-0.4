// Sonido.h
#ifndef SONIDO_H
#define SONIDO_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <map>
#include <iostream>

class Sonido {
public:
    static Sonido& ObtenerInstancia() {
        static Sonido instancia;
        return instancia;
    }
    Sonido(const Sonido&) = delete;
    Sonido& operator=(const Sonido&) = delete;

    // Carga una música (archivo .mp3 u otro) para fondo.
    bool cargarMusica(const std::string& etiqueta, const std::string& rutaArchivo) {
        Mix_Music* musica = Mix_LoadMUS(rutaArchivo.c_str());
        if (!musica) {
            std::cerr << "Error al cargar la música (" << rutaArchivo << "): "
                << Mix_GetError() << std::endl;
            return false;
        }
        musicas_[etiqueta] = musica;
        return true;
    }

    // Carga un efecto de sonido.
    bool cargarEfecto(const std::string& etiqueta, const std::string& rutaArchivo) {
        Mix_Chunk* efecto = Mix_LoadWAV(rutaArchivo.c_str());
        if (!efecto) {
            std::cerr << "Error al cargar el efecto (" << rutaArchivo << "): "
                << Mix_GetError() << std::endl;
            return false;
        }
        efectos_[etiqueta] = efecto;
        return true;
    }

    // Reproduce la música de fondo.
    void reproducirMusica(const std::string& etiqueta, int loops = 0) {
        auto it = musicas_.find(etiqueta);
        if (it != musicas_.end()) {
            if (Mix_PlayMusic(it->second, loops) == -1) {
                std::cerr << "Error al reproducir la música (" << etiqueta << "): "
                    << Mix_GetError() << std::endl;
            }
        }
        else {
            std::cerr << "Música con etiqueta \"" << etiqueta << "\" no encontrada." << std::endl;
        }
    }

    // Reproduce un efecto de sonido en un canal determinado.
    void reproducirEfecto(const std::string& etiqueta, int loops = 0, int canal = -1) {
        auto it = efectos_.find(etiqueta);
        if (it != efectos_.end()) {
            if (Mix_PlayChannel(canal, it->second, loops) == -1) {
                std::cerr << "Error al reproducir el efecto (" << etiqueta << "): "
                    << Mix_GetError() << std::endl;
            }
        }
        else {
            std::cerr << "Efecto con etiqueta \"" << etiqueta << "\" no encontrado." << std::endl;
        }
    }

    void liberarMusica(const std::string& etiqueta) {
        auto it = musicas_.find(etiqueta);
        if (it != musicas_.end()) {
            Mix_FreeMusic(it->second);
            musicas_.erase(it);
        }
    }

    void liberarEfecto(const std::string& etiqueta) {
        auto it = efectos_.find(etiqueta);
        if (it != efectos_.end()) {
            Mix_FreeChunk(it->second);
            efectos_.erase(it);
        }
    }

    void liberarTodos() {
        for (auto& par : musicas_) {
            Mix_FreeMusic(par.second);
        }
        musicas_.clear();
        for (auto& par : efectos_) {
            Mix_FreeChunk(par.second);
        }
        efectos_.clear();
    }

private:
    Sonido() {}
    ~Sonido() {
        liberarTodos();
    }

    std::map<std::string, Mix_Music*> musicas_;
    std::map<std::string, Mix_Chunk*> efectos_;
};

#endif // SONIDO_H
