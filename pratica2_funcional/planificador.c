#include "planificador.h"

void wait0(void)  { /* wait function */
	unsigned int i;

	i = timer0_read_int_count(); // reads the number of previous timer IRQs
	while ((i + 10) != timer0_read_int_count());  /* waits for 10 interrupts, i.e. 50ms */
}


//Se encargará de iniciar la cola 
void planificador() {
    FIFO_inicializar(GPIO_OVERFLOW);
    hello_world_inicializar();

    while (!gpio_hal_leer(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS)) {
        temporizador_drv_reloj(10, &FIFO_encolar, Timer1);
        wait0();
        hello_world_tick_tack();
    }
    gpio_hal_escribir(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS, 0);
}
