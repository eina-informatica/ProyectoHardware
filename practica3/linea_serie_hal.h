
#include "evento.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "gpio_hal.h"
#include "tablero.h"
#include <LPC210X.H> // LPC21XX Peripheral Registers

#ifndef LINEA_SERIE_HAL_H
#define LINEA_SERIE_HAL_H
void uart0_init(void (*funcion_callback)(char),void (*callback)(EVENTO_T, uint32_t),uint32_t err_pin,uint32_t err_size,void (*callback_a)(EVENTO_T, uint32_t, uint32_t));
void linea_serie_hal_enviar_array(char cadena[]);
void linea_serie_hal_continuar_envio(void);

void gestion_caracter_hal(char c);
void uart0_ISR(void) __irq;
static int i=0;
static int ini_array=0;
static int indice=0;
enum estado_recibir{
	inicio,
	durante
};
static char buffer_envio[200];

enum estado_envio {
 ocupado,
 libre
};


#endif
