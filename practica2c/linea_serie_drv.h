#include "linea_serie_hal.h"

#ifndef LINEA_SERIE_DRV_H
#define LINEA_SERIE_DRV_H
void gestion_caracter(char c);
void linea_serie_drv_iniciar(void);
void linea_serie_drv_enviar_array(char cadena[]);
void linea_serie_drv_continuar_envio(void);

#endif
