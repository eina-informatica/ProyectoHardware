#include "power.h"

#ifndef GESTOR_ENERGIA_H
#define GESTOR_ENERGIA_H

// Enumeración para representar el estado del procesador
enum estado_procesador {
    ENCENDIDO,
    APAGADO
};

static enum estado_procesador estado = ENCENDIDO; // Declaración de la variable de estado inicializada a ENCENDIDO

// Declaración de funciones relacionadas con la gestión de energía
void energia_idle(void);
void energia_power_down(void);
unsigned int energia_esta_encendido(void);
void energia_cambiar_a_encendido(void);

// Fin del archivo de encabezado
#endif
