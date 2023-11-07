#include "gpio_hal.h"
#include "juego.h"
#include <stdint.h>

#ifndef VISUALIZAR_H
#define VISUALIZAR_H

static GPIO_HAL_PIN_T INI_Visualizar, Tam_Visualizar;

void inicializar_visualizar(void);
void visualizar(uint32_t cuenta);
#endif
