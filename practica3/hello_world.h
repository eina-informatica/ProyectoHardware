#include <stddef.h>
#include "gpio_hal.h"
#include "evento.h"

#ifndef HELLO_WORLD_H
#define HELLO_WORLD_H

static int pin, size, tick_count, tick_count_decimas;

void hello_world_inicializar(void (*callback)(EVENTO_T, uint32_t, uint32_t), uint32_t gpio_hello_world, uint32_t gpio_hello_world_bits, gpio_hal_pin_dir_t dir,void (*callback_2)(GPIO_HAL_PIN_T, uint8_t, gpio_hal_pin_dir_t));
void hello_world_tick_tack(void (*callback)(GPIO_HAL_PIN_T, uint8_t, uint32_t));
void hello_world_tratar_evento(void (*callback)(EVENTO_T, uint32_t)); 

#endif
