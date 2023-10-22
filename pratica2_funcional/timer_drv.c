#include "timer_drv.h"

EVENTO_T evento_temp;

void (*callback_func_temp)(EVENTO_T, uint32_t);

void temporizador_drv_iniciar(){
    temporizador_hal_iniciar();
}

void temporizador_drv_empezar(){
    temporizador_hal_empezar();
}

uint64_t temporizador_drv_leer(){
    return TIMER_HAL_TICKS2US * temporizador_hal_leer();
}

uint64_t temporizador_drv_parar(){
    return TIMER_HAL_TICKS2US * temporizador_hal_parar();
}

void aux_callback() {
    if (callback_func_temp != NULL) {
        callback_func_temp(evento_temp, 0);
    }
}

void temporizador_drv_reloj(uint32_t periodo, void (*funcion_encolar_evento)(), EVENTO_T ID_evento) {
    evento_temp = ID_evento;
    callback_func_temp = funcion_encolar_evento;

    temporizador_hal_reloj(periodo, &aux_callback);
}
