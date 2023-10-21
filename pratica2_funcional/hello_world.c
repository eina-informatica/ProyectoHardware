#include "hello_world.h"

int pin, size, cont;

void hello_world_inicializar() {
    pin = GPIO_HELLO_WORLD;
    size = GPIO_HELLO_WORLD_BITS;
    cont = 0;
}

void hello_world_tick_tack() {
    gpio_hal_sentido(0, 8,GPIO_HAL_PIN_DIR_OUTPUT);
    gpio_hal_sentido(0, 8,GPIO_HAL_PIN_DIR_INPUT);
    for (cont = pin; i < size; cont++)
    {
        temporizador_drv_reloj(10, NULL, NULL);
        wait();
        gpio_hal_escribir(cont, 1, 1);
        cont++;
    }
    gpio_hal_escribir(pin, size, 0);
}