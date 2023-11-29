#include "gpio_hal.h"
#include <stdint.h>
#ifndef VISUALIZAR_H
#define VISUALIZAR_H

void inicializar_visualizar(uint32_t gpio_visu, uint32_t gpio_visu_bits, gpio_hal_pin_dir_t direccion,void (*callback_2)(GPIO_HAL_PIN_T, uint8_t, gpio_hal_pin_dir_t),void (*callback)(GPIO_HAL_PIN_T, uint8_t, uint32_t));
void visualizar(uint32_t cuenta);

#endif
