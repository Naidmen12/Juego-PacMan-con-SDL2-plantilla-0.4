#include "Motor.h"
#include "GestorEventos.h"
#include "ControlColision.h"
#include "Menus.h"
#include "ControlSonidos.h"

constexpr auto ANCHOP = 800;
constexpr auto ALTOP = 640;

Entidad* Personaje = nullptr;
GestorEventos* gestoreventos = nullptr;
GestorEventos* gestorMenus = nullptr;
Mapa* mapa = nullptr;
ControlColision* controlcolisiones = nullptr;
Enemigo* fantasma1 = nullptr;
Enemigo* fantasma2 = nullptr;
Animacion* animacionPersonaje = nullptr;
Menus* Puntaje = nullptr;
Menus* menuEntrada = nullptr;
Menus* menuCJuego = nullptr;
Menus* menuGano = nullptr;
Menus* menuPerdio = nullptr;
ControlSonidos* controlSonidos = nullptr;

Motor::Motor(int fps) : FPS(fps), Estado(0), Recorrido(0) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        corriendo = false;
    }
    else {
        ventana = SDL_CreateWindow("Motor SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ANCHOP, ALTOP, 0);
        if (ventana == nullptr) {
            std::cerr << "Error al crear la ventana: " << SDL_GetError() << std::endl;
            corriendo = false;
        }
        else {
            renderizador = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);
            if (renderizador == nullptr) {
                std::cerr << "Error al crear el renderizador: " << SDL_GetError() << std::endl;
                corriendo = false;
            }
            else {
                corriendo = true;
            }
        }
    }
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG; // Inicializa los formatos PNG y JPG
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "Error al inicializar SDL_image: " << IMG_GetError() << std::endl;  
    }
    if (TTF_Init() == -1) {
        std::cerr << "Error al inicializar SDL_ttf: " << TTF_GetError() << std::endl;
        // Maneja el error o finaliza la aplicación
    }
    // Inicialización de SDL_mixer (asegúrate de tener soporte para MP3).
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) == -1) {
        std::cerr << "Error al inicializar SDL_mixer: " << Mix_GetError() << std::endl;
    }
    Mix_AllocateChannels(16);
}

Motor::~Motor() {
    EliminarTodo();
}

void Motor::Inicializar(){
    //Variables
    SDL_Rect srcRectP;
    SDL_Rect destRectP;
    SDL_Color Color;
    //Cargar texturas
    if (!ManejarTextura::Instancia()->Cargar("Contenido/Personaje.png","Personaje",renderizador)) {
        std::cout << "No se cargo la textura del personaje" << std::endl;
    }
    if (!ManejarTextura::Instancia()->Cargar("Contenido/Enemigo.png", "Fantasma", renderizador)) {
        std::cout << "No se cargo la textura del personaje" << std::endl;
    }
    if (!ManejarTextura::Instancia()->Cargar("Contenido/Menu.png", "Menu", renderizador)) {
        std::cout << "No se cargo la textura del personaje" << std::endl;
    }
	//Sonidos
	if (!Sonido::ObtenerInstancia().cargarMusica("FondoM","Contenido/FondoM.mp3")) {
		std::cout << "No se cargo la musica" << std::endl;
	}
    if (!Sonido::ObtenerInstancia().cargarMusica("FondoM2", "Contenido/FondoM2.mp3")) {
        std::cout << "No se cargo la musica" << std::endl;
    }
    if (!Sonido::ObtenerInstancia().cargarEfecto("Waka", "Contenido/SonidoPac.mp3")) {
        std::cout << "No se cargo la musica" << std::endl;
    }
    //Objetos
    srcRectP = { 0, 0, 32, 32 };
    destRectP = { 32, 32, 30, 30 };
    Personaje = new Entidad("Personaje", destRectP, srcRectP);
    srcRectP = { 0, 0, 32, 32 };
    destRectP = { 352, 320, 32, 32 };
    fantasma1 = new Enemigo("Fantasma", destRectP, srcRectP, 2.0f);
    srcRectP = { 0, 0, 32, 32 };
    destRectP = { 448, 320, 32, 32 };
    fantasma2 = new Enemigo("Fantasma", destRectP, srcRectP, 2.0f);
    //Animaciones
    animacionPersonaje = new Animacion("Personaje", Personaje->ObtenerScrRect(), 5);
    //Manejo de eventos o entrada
    gestoreventos = new GestorEventos(Personaje, animacionPersonaje, nullptr);
    //Mapas
    mapa = new Mapa(renderizador);
    if (!mapa->CargarMapa("Contenido/MapaP.tmx", "Contenido/TMapa.png")) {
        std::cerr << "Error al cargar el mapa" << std::endl;
    }
    //Colisiones
    controlcolisiones = new ControlColision();
    //Textos
    if (!Texto::Instancia()->Cargar("Contenido/Crackman.TTF","PacmanF",24)) {
        std::cout << "No se cargo la fuente de texto" << std::endl;
    }
    //Menu Puntaje
    destRectP = { 0, 0, 320, 32 };
    Color = { 0,0,0 };
    Puntaje = new Menus(renderizador, "PacmanF", destRectP, Color);
    Puntaje->agregarOpcion("Puntaje: ", controlcolisiones->ObtenerContador());
    srcRectP = { 0, 0, 32, 32 };
    destRectP = { 0, 0, 192, 32 };
    Puntaje->AgregarObjeto(srcRectP, destRectP, "Menu");
    //Menu entrada
    destRectP = { (ANCHOP / 2) - (((ANCHOP / 32) * 6)/2), (ALTOP / 2) - (((ALTOP / 32) * 2) / 2), (ANCHOP / 32) * 6, (ALTOP / 32) * 1 };
    Color = { 0,0,0 };
    menuEntrada = new Menus(renderizador, "PacmanF", destRectP, Color);
    menuEntrada->darModo(ModoMenu::LISTA);
    menuEntrada->agregarOpcion("Iniciar", 1);
    menuEntrada->agregarOpcion("Salir", 5);
    srcRectP = { 0, 0, 32, 32 };
    destRectP = { (ANCHOP / 2) - (((ANCHOP / 32) * 12) / 2), (ALTOP / 2) - (((ALTOP / 32) * 2) / 2), 32, 32 };
    menuEntrada->AgregarObjeto(srcRectP, destRectP, "Personaje");
    destRectP = { (ANCHOP / 2) - (((ANCHOP / 32) * -9) / 2), (ALTOP / 2) - (((ALTOP / 32) * 2) / 2), 32, 32 };
    menuEntrada->AgregarObjeto(srcRectP, destRectP, "Personaje");
    destRectP = { (ANCHOP / 2) - (((ANCHOP / 32) * 12) / 2), (ALTOP / 2) - (((ALTOP / 32) * -4) / 2), 32, 32 };
    menuEntrada->AgregarObjeto(srcRectP, destRectP, "Fantasma");
    destRectP = { (ANCHOP / 2) - (((ANCHOP / 32) * -9) / 2), (ALTOP / 2) - (((ALTOP / 32) * -4) / 2), 32, 32 };
    menuEntrada->AgregarObjeto(srcRectP, destRectP, "Fantasma");
    gestorMenus = new GestorEventos(nullptr, nullptr, menuEntrada);
    //Menu empezar juego
    destRectP = { (ANCHOP / 2) - (((ANCHOP / 32) * 14) / 2), (ALTOP / 2) - (((ALTOP / 32) * 2) / 2), (ANCHOP / 32) * 16, (ALTOP / 32) * 1 };
    Color = { 0,0,0 };
    menuCJuego = new Menus(renderizador, "PacmanF", destRectP, Color);
    menuCJuego->agregarOpcion("Comenzando el juego en el Mapa", -1);
    srcRectP = { 0, 0, 32, 32 };
    destRectP = { (ANCHOP / 2) - (((ANCHOP / 32) * 14) / 2), (ALTOP / 2) - (((ALTOP / 32) * 2) / 2), (ANCHOP / 32) * 18, (ALTOP / 32) * 1 };
    menuCJuego->AgregarObjeto(srcRectP, destRectP, "Menu");
    //Menu gano juego
    destRectP = { (ANCHOP / 2) - (((ANCHOP / 32) * 14) / 2), (ALTOP / 2) - (((ALTOP / 32) * 2) / 2), (ANCHOP / 32) * 16, (ALTOP / 32) * 1 };
    Color = { 0,0,0 };
    menuGano = new Menus(renderizador, "PacmanF", destRectP, Color);
    menuGano->agregarOpcion("Haz ganado el juego, felicidades", -1);
    srcRectP = { 0, 0, 32, 32 };
    destRectP = { (ANCHOP / 2) - (((ANCHOP / 32) * 14) / 2), (ALTOP / 2) - (((ALTOP / 32) * 2) / 2), (ANCHOP / 32) * 18, (ALTOP / 32) * 1 };
    menuGano->AgregarObjeto(srcRectP, destRectP, "Menu");
    //Menu perdio juego
    destRectP = { (ANCHOP / 2) - (((ANCHOP / 32) * 14) / 2), (ALTOP / 2) - (((ALTOP / 32) * 2) / 2), (ANCHOP / 32) * 16, (ALTOP / 32) * 1 };
    Color = { 0,0,0 };
    menuPerdio = new Menus(renderizador, "PacmanF", destRectP, Color);
    menuPerdio->agregarOpcion("Haz perdido el juego", -1);
    srcRectP = { 0, 0, 32, 32 };
    destRectP = { (ANCHOP / 2) - (((ANCHOP / 32) * 14) / 2), (ALTOP / 2) - (((ALTOP / 32) * 2) / 2), (ANCHOP / 32) * 18, (ALTOP / 32) * 1 };
    menuPerdio->AgregarObjeto(srcRectP, destRectP, "Menu");
	//Control de sonidos
	controlSonidos = new ControlSonidos();
}

void Motor::Eventos() {
    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_QUIT) {
            corriendo = false;
        }
    }
}

void Motor::Actualizar() {
    controlSonidos->Actualizar();
    switch (Estado) {
    case 0:
        Estado = gestorMenus->TecladoMenu();
		controlSonidos->reproducirControlado("FondoM", -1);
        break;
    case 1: {
        static bool firstEntry = true;
        static Uint32 pausaInicio = 0;
        if (firstEntry) {
            pausaInicio = SDL_GetTicks();
            firstEntry = false;
        }
        // Si han pasado 5000 ms, se retoma el juego (cambiando al estado 2)
        if (SDL_GetTicks() - pausaInicio >= 5000) {
            Estado = 2;
            firstEntry = true; // Reinicializamos para la próxima pausa, si fuera necesario
        }
        break;
    }
    case 2:
        //Actualizar del juego
        controlSonidos->reproducirControlado("FondoM2", -1);
        //Eventos de teclado
        gestoreventos->ManejarEventos();
        //Eventos de colisiones
        controlcolisiones->ComprobarColision(*Personaje, *mapa, 2);
        controlcolisiones->ColisionEQ(Personaje, mapa, 1);
        //Objetos
        Personaje->Actualizar();
        fantasma1->LogicaEnemigo(*mapa, *Personaje, 2);
        fantasma2->LogicaEnemigo(*mapa, *Personaje, 2);
        //Menus
        Puntaje->actualizarOpcionConVariable(controlcolisiones->ObtenerContador());
        if (controlcolisiones->ObtenerContador() == mapa->RetornarContado()) {
            Estado = 4;
        }
        if (controlcolisiones->ColisionEnemigo(*Personaje, *fantasma1) || controlcolisiones->ColisionEnemigo(*Personaje, *fantasma2)) {
            Estado = 3;
        }
        break;
    case 3: {
        // Estado 4: "Congela" el juego durante 5 segundos, luego destruye y recrea todos los objetos, y pone Estado = 0.
        static bool firstEntry = true;
        static Uint32 freezeInicio = 0;
        if (firstEntry) {
            freezeInicio = SDL_GetTicks();
            firstEntry = false;
        }
        // Durante estos 5 segundos no se actualiza nada (se "congela")
        if (SDL_GetTicks() - freezeInicio >= 5000) {
            // Se destruyen todos los objetos
            EliminarObjetos();
            // Se recrean los objetos (asumiendo que Inicializar() los crea nuevamente)
            Inicializar();
            // Se cambia el estado a 0 para, por ejemplo, volver al menú principal
            Estado = 0;
            // Reiniciamos la variable para poder usarla la próxima vez que se active esta función
            firstEntry = true;
        }
        break;
    }
    case 4: {
        // Estado 4: "Congela" el juego durante 5 segundos, luego destruye y recrea todos los objetos, y pone Estado = 0.
        static bool firstEntry = true;
        static Uint32 freezeInicio = 0;
        if (firstEntry) {
            freezeInicio = SDL_GetTicks();
            firstEntry = false;
        }
        // Durante estos 5 segundos no se actualiza nada (se "congela")
        if (SDL_GetTicks() - freezeInicio >= 5000) {
            // Se destruyen todos los objetos
            EliminarObjetos();
            // Se recrean los objetos (asumiendo que Inicializar() los crea nuevamente)
            Inicializar();
            // Se cambia el estado a 0 para, por ejemplo, volver al menú principal
            Estado = 0;
            // Reiniciamos la variable para poder usarla la próxima vez que se active esta función
            firstEntry = true;
        }
        break;
    }
    case 5:
        corriendo = false;
        break;
    }
}

void Motor::Imprimir() {
    SDL_SetRenderDrawColor(renderizador, 50, 0, 100, 255);
    SDL_RenderClear(renderizador);
    switch (Estado){
    case 0:
        menuEntrada->dibujarMenu();
        break;
    case 1:
        menuCJuego->dibujarMenu();
        break;
    case 2:
        //Renderizado del juego
        mapa->RenderizarMapa();
        Personaje->Dibujar(renderizador);
        fantasma1->Dibujar(renderizador);
        fantasma2->Dibujar(renderizador);
        Puntaje->dibujarMenu();
        break;
    case 3:
        menuPerdio->dibujarMenu();
        break;
    case 4:
        menuGano->dibujarMenu();
        break;
    }
    SDL_RenderPresent(renderizador);
}

void Motor::ControlarFrames() {
    Uint32 tiempoAhora = SDL_GetTicks();
    deltaTime = (tiempoAhora - tiempoAnterior) / 1000.0f;  // Calcula el deltaTime en segundos
    tiempoAnterior = tiempoAhora;

    // Acumulamos el tiempo transcurrido
    tiempoAcumulado += deltaTime;

    // Limitar el tiempo acumulado para evitar que se desborde en caso de que se tarde mucho entre frames
    if (tiempoAcumulado > 1.0f) {
        tiempoAcumulado = 1.0f;
    }

    // Controlamos la actualización lógica a una tasa constante
    while (tiempoAcumulado >= (1.0f / FPS)) {
        Actualizar();  // Actualizamos la lógica
        tiempoAcumulado -= (1.0f / FPS);
    }
}

void Motor::EliminarTodo() {
    EliminarObjetos();
    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(ventana);
    TTF_Quit();
    Mix_CloseAudio();
    SDL_Quit();
}

void Motor::EliminarObjetos() {
    if (Personaje) {
        delete Personaje;
        Personaje = nullptr;
    }
    if (fantasma1) {
        delete fantasma1;
        fantasma1 = nullptr;
    }
    if (fantasma2) {
        delete fantasma2;
        fantasma2 = nullptr;
    }
    if (animacionPersonaje) {
        delete animacionPersonaje;
        animacionPersonaje = nullptr;
    }
    if (gestoreventos) {
        delete gestoreventos;
        gestoreventos = nullptr;
    }
    if (gestorMenus) {
        delete gestorMenus;
        gestorMenus = nullptr;
    }
    if (mapa) {
        delete mapa;
        mapa = nullptr;
    }
    if (controlcolisiones) {
        delete controlcolisiones;
        controlcolisiones = nullptr;
    }
    if (Puntaje) {
        delete Puntaje;
        Puntaje = nullptr;
    }
    if (menuEntrada) {
        delete menuEntrada;
        menuEntrada = nullptr;
    }
    if (menuCJuego) {
        delete menuCJuego;
        menuCJuego = nullptr;
    }
    if (menuGano) {
        delete menuGano;
        menuGano = nullptr;
    }
    if (menuPerdio) {
        delete menuPerdio;
        menuPerdio = nullptr;
    }
	if (controlSonidos) {
		delete controlSonidos;
		controlSonidos = nullptr;
	}

    // Limpia las texturas (si ManejarTextura::Instancia() gestiona recursos compartidos, 
    // asegúrate de llamarlo solo una vez)
	Sonido::ObtenerInstancia().liberarTodos();
    ManejarTextura::Instancia()->Limpiar();
}
