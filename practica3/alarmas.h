#include <inttypes.h>
#include "evento.h"

#ifndef ALARMAS_H
#define ALARMAS_H

#define MAX_ALARM 4

void alarma_inicializar(void(*funcion_callback)(EVENTO_T, uint32_t));
void alarma_activar(EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData);
void alarma_tratar_evento(void);


#endif
