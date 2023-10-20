#include <LPC210X.H>

#include <stdint.h>

// Constante que permite convertir de ticks a microsegundos
#define TIMER_HAL_TICKS2US (1.0 / (CCLK / 1000000.0))

// Funciones
void temporizador_hal_iniciar();
void temporizador_hal_empezar();
uint64_t temporizador_hal_leer();
uint64_t temporizador_hal_parar();  