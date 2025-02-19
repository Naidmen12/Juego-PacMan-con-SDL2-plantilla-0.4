#include "Motor.h"

int main(int argc, char* args[]) {
    Motor motor(60);
    motor.Inicializar();

    while (motor.corriendo) {
        motor.Eventos();
        motor.Imprimir();
        motor.ControlarFrames();
    }

    motor.EliminarTodo();
    return 0;
}
