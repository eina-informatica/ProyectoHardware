#include "gestor_energia.h" // Incluye el archivo de encabezado G_Energia.h

void energia_idle(void) {
    power_hal_wait(); // Llama a la función power_hal_wait()  para poner el procesador en estado de espera o bajo consumo de energía
}

void energia_power_down(void) {
    estado = APAGADO; // Cambia el estado a APAGADO
    power_hal_deep_sleep(); // Llama a la función power_hal_deep_sleep() (presumiblemente para apagar o poner en modo bajo consumo de energía)
}

unsigned int energia_esta_encendido(void) {
    return estado == ENCENDIDO; // Devuelve 1 (verdadero) si el estado es ENCENDIDO, 0 (falso) en caso contrario
}

void energia_cambiar_a_encendido(void) {
    estado = ENCENDIDO; // Cambia el estado a ENCENDIDO
}
