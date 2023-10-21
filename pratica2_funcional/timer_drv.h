#include <stddef.h>
#include "timer_hal.h"
#include "cola.h"

// Funciones
void temporizador_drv_iniciar(void);
void temporizador_drv_empezar(void);
uint64_t temporizador_drv_leer(void);
uint64_t temporizador_drv_parar(void);
void temporizador_drv_reloj(uint32_t periodo, void (*funcion_encolar_evento)(), EVENTO_T ID_evento);