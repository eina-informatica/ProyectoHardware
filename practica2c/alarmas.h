#include <inttypes.h>
#include "cola.h"

#ifndef ALARMAS_H
#define ALARMAS_H

#define MAX_ALARM 4

typedef struct {
    uint8_t periodica;
		uint8_t activa;
		EVENTO_T id;
    uint32_t retardo;
		uint32_t auxData;
		
} Alarma;

static Alarma alarmas[MAX_ALARM];
static unsigned int periodo = 10;
static unsigned int duracion[MAX_ALARM];

void alarma_inicializar(void);
void alarma_activar(EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData);
void alarma_tratar_evento(void);

#endif
