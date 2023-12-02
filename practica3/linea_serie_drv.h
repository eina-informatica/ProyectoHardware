#include "linea_serie_hal.h"
#include "tablero.h"

#ifndef LINEA_SERIE_DRV_H
#define LINEA_SERIE_DRV_H

void gestion_caracter(char c);
void linea_serie_drv_iniciar(void (*callback)(EVENTO_T, uint32_t), uint32_t ser_err_pin, uint32_t ser_err_size, uint32_t com_err_pin, uint32_t com_err_size, void (*callback_a)(EVENTO_T, uint32_t, uint32_t));
void linea_serie_drv_enviar_array(char cadena[]);
void linea_serie_drv_continuar_envio(void);

#endif
