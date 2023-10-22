#include "planificador.h"

int stat;

void wait0(void)  { /* wait function */
	unsigned int i;

	i = timer0_read_int_count(); // reads the number of previous timer IRQs
	while ((i + 10) != timer0_read_int_count());  /* waits for 10 interrupts, i.e. 50ms */
}


//Se encargará de iniciar la cola 
void planificador() {
		temporizador_drv_iniciar();
		temporizador_drv_empezar();
		gpio_hal_sentido(0, 32, GPIO_HAL_PIN_DIR_OUTPUT);
		//gpio_hal_escribir(0, 32, 0);
	
    FIFO_inicializar(GPIO_OVERFLOW);
    hello_world_inicializar();

    while (!gpio_hal_leer(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS)) {
        temporizador_drv_reloj(10, &FIFO_encolar, Timer1);
        wait0();
        hello_world_tick_tack();
				stat = FIFO_estadisticas(Timer1);
    }
    gpio_hal_escribir(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS, 0);
}
