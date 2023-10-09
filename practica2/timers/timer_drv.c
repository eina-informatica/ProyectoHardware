#include "timer_drv.h"

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
