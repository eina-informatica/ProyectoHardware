#include <stdint.h>
#include "int_externas_hal.h"
#include "evento.h"

#ifndef BOTONES_H
#define BOTONES_H



static uint32_t USUARIO_AUSENTE=8400608;//---> 12 seg * 0x800000 para activar periocidad
void boton_init(void (*callback_f)(EVENTO_T, uint32_t),void (*callback_a)(EVENTO_T, uint32_t, uint32_t));

void boton_pulsado(uint8_t ID_evento);

void boton_comprobar_pulsacion(uint8_t boton);

#endif
