#include <LPC210X.H>
#include <stdint.h>
#ifndef GPIO_H
#define GPIO_H

typedef enum {
    GPIO_HAL_PIN_DIR_INPUT,
    GPIO_HAL_PIN_DIR_OUTPUT
} gpio_hal_pin_dir_t;

typedef uint8_t GPIO_HAL_PIN_T;

__inline void gpio_hal_iniciar();
__inline void gpio_hal_sentido(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, gpio_hal_pin_dir_t direccion);
__inline uint32_t gpio_hal_leer(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits);
__inline void gpio_hal_escribir(GPIO_HAL_PIN_T bit_inicial, uint8_t num_bits, uint32_t valor);
 #endif
