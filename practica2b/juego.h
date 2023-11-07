#include <stdint.h>
#include "cola.h"
#include "timer_drv.h"

#ifndef JUEGO_H
#define JUEGO_H

static uint32_t cuenta, intervalo, ant_intervalo;

void juego_inicializar(void);
void juego_tratar_evento(EVENTO_T ID_evento, uint32_t auxData);
int juego_leer_cuenta(void);
int juego_leer_intervalo(void);

#endif
