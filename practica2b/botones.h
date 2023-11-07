#include <stdint.h>
#include "int_externas_hal.h"
#include "alarmas.h"
#include "gpio_hal.h"

#ifndef BOTONES_H
#define BOTONES_H

static uint32_t cienMS_period=8388708; // Agrega un punto y coma al final de la declaración.

/* Creamos maquina de estados como un enum de 2 estados */

typedef enum estadoBoton {
    DOWN=0,
    UP=1
}estadoBoton;
static uint32_t USUARIO_AUSENTE=8400608;//---> 12 seg * 0x800000 para activar periocidad


static estadoBoton estado1=DOWN; // Asigna los valores iniciales DOWN a las variables estado1 y estado2.
static estadoBoton estado2=DOWN;

void boton_init(void);

void boton_pulsado(uint8_t ID_evento);

void boton_comprobar_pulsacion(uint8_t boton);

#endif
