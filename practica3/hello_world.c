#include "hello_world.h"

static int pin, size, tick_count, tick_count_decimas;

void hello_world_inicializar(void (*callback)(EVENTO_T, uint32_t, uint32_t), uint32_t gpio_hello_world, uint32_t gpio_hello_world_bits, gpio_hal_pin_dir_t dir,void (*callback_2)(GPIO_HAL_PIN_T, uint8_t, gpio_hal_pin_dir_t)) {
	pin = gpio_hello_world;
	size = gpio_hello_world_bits;
	callback_2(0, 8, dir);
	callback(ev_LATIDO, 8388618, 0); // Cada 10ms
	tick_count = 0;
	tick_count_decimas = 0;
}

void hello_world_tick_tack(void (*callback)(GPIO_HAL_PIN_T, uint8_t, uint32_t)) {
	tick_count++;
	if (tick_count >= 9) {
		tick_count_decimas++;
		callback(pin, size, (tick_count_decimas % 128));
		tick_count = 0;
	}
}

void hello_world_tratar_evento(void (*callback)(EVENTO_T, uint32_t)) {
	callback(ev_VISUALIZAR_HELLO, 0);
}
