#include "hello_world.h"

void hello_world_inicializar(void) {
    pin = GPIO_HELLO_WORLD;
    size = GPIO_HELLO_WORLD_BITS;
    gpio_hal_sentido(0, 8, GPIO_HAL_PIN_DIR_OUTPUT);
    alarma_activar(ev_LATIDO, 8388618,0);//Cada 10ms 
		tick_count = 0;
	  tick_count_decimas = 0;
    //gpio_hal_sentido(0, 8, GPIO_HAL_PIN_DIR_INPUT);
}

void hello_world_tick_tack(void) {
    tick_count++;
		if(tick_count >=9){
			tick_count_decimas++;
			gpio_hal_escribir(GPIO_HELLO_WORLD,GPIO_HELLO_WORLD_BITS,(tick_count_decimas%128));
			tick_count=0;
		}
}

void hello_world_tratar_evento(void) {
   FIFO_encolar(ev_VISUALIZAR_HELLO, 0);
}

