#include "hello_world.h"

int pin, size, cont;

void hello_world_inicializar(void) {
    pin = GPIO_HELLO_WORLD;
    size = GPIO_HELLO_WORLD_BITS;
    cont = pin;
    gpio_hal_sentido(0, 8, GPIO_HAL_PIN_DIR_OUTPUT);
    gpio_hal_sentido(0, 8, GPIO_HAL_PIN_DIR_INPUT);
}

void hello_world_tick_tack(void) {
    if (cont < size ){
        gpio_hal_escribir(cont, 1, 1);
        cont++;
    }
}
