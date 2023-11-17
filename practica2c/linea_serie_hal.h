#include "io_reserva.h"
#include "cola.h"
#include "gpio_hal.h"
#include <string.h>
#include <ctype.h>
#include <LPC210X.H> // LPC21XX Peripheral Registers

#ifndef LINEA_SERIE_HAL_H
#define LINEA_SERIE_HAL_H
void uart0_init(void (*funcion_callback)(char));
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
static char buffer[3];
enum estado_envio {
 ocupado,
 libre
};


#endif
