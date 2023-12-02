
#include "evento.h"
#include <string.h>
#include <ctype.h>
#include "gpio_hal.h"
#include <stdlib.h>
#include <LPC210X.H> // LPC21XX Peripheral Registers

#ifndef LINEA_SERIE_HAL_H
#define LINEA_SERIE_HAL_H
void uart0_init(void (*funcion_callback)(char),void (*callback)(EVENTO_T, uint32_t));
void linea_serie_hal_enviar_array(char cadena[]);
void linea_serie_hal_continuar_envio(void);
void uart0_ISR(void) __irq;

#endif
