#include "planificador.h"
#include "colillas/cola.h"
#include "io_reserva.h"
#include "gpio_hal.h"

//Se encargará de iniciar la cola 
void planificador_iniciar(){
    gpio_hal_pin_dir_t pin= GPIO_OVERFLOW;
    FIFO_inicializar(pin);
}