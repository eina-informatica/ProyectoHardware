#include <LPC210X.H> // LPC21XX Peripheral Registers
#include <stdint.h>
#include <stddef.h>
#include "SWI.h"

#ifndef TIMER_HAL_H
#define TIMER_HAL_H


// Constante que permite convertir de ticks a microsegundos
//#define TIMER_HAL_TICKS2US (1.0 / (CCLK / 1000000.0))
#define TIMER_HAL_TICKS2US (15)

// Funciones
void temporizador_hal_iniciar(void);
void temporizador_hal_empezar(void);
uint64_t temporizador_hal_leer(void);
uint64_t temporizador_hal_parar(void);
void temporizador_hal_reloj(uint32_t periodo, void (*funcion_callback)());
void gestion_caracter_hal(char c);

#endif
