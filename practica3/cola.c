#include "cola.h"

struct ITEM cola[SIZE];
int numV[max_Eventos];
uint8_t frente;
uint8_t final;
uint8_t i; 
GPIO_HAL_PIN_T gpio_overflow;
uint8_t gpio_overflow_bits;
gpio_hal_pin_dir_t sent;
static uint32_t bit_IRQ;
// Estructura para un elemento de la cola
// Inicialización de la cola FIFO
static void (*callback_escribir)(GPIO_HAL_PIN_T, uint8_t, uint32_t);
static void (*callback_sentido)(GPIO_HAL_PIN_T, uint8_t, gpio_hal_pin_dir_t);

void FIFO_inicializar(GPIO_HAL_PIN_T gpio_of, uint8_t gpio_of_bits, gpio_hal_pin_dir_t sentido, void (*callback_e)(GPIO_HAL_PIN_T, uint8_t, uint32_t), void (*callback_s)(GPIO_HAL_PIN_T, uint8_t, gpio_hal_pin_dir_t)) {
    bit_IRQ=read_irq_bit();
    frente = 0; // lgo sumar ++ eb las iter
    final = 0;  
    sent = sentido;
	if (callback_e != NULL && callback_s != NULL) {
    callback_escribir = callback_e;
    callback_sentido = callback_s;
	}
    gpio_overflow = gpio_of;
    gpio_overflow_bits = gpio_of_bits;
    callback_sentido(gpio_overflow, gpio_overflow_bits, sent);
    callback_escribir(gpio_overflow, gpio_overflow_bits, 0);
    for (i = 0; i < max_Eventos; i++) {
        numV[i] = 0;
    }
}

// Función para encolar un evento
void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData) {
    bit_IRQ=read_irq_bit();
    if (!bit_IRQ)
    {
        disable_irq();
    }
    if ((final + 1) % SIZE != frente) {
        cola[final].id =ID_evento;
        cola[final].auxDATA = auxData;
        numV[ID_evento]++;
        final = (final + 1) % SIZE;

    } else {
		callback_sentido(gpio_overflow, gpio_overflow_bits, sent);
        callback_escribir(gpio_overflow, gpio_overflow_bits, 1);
        // La cola está llena, manejo de error o desbordamiento 
			while(1);
    }
    if (bit_IRQ)
    {
        enable_irq();
    } 
}

// Función para extraer un evento de la cola
uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData) {
    bit_IRQ=read_irq_bit();
    if (!bit_IRQ)
    {
        disable_irq();
    }
    if (frente != final) {
			*ID_evento = cola[frente].id;
			*auxData = cola[frente].auxDATA;
			frente = (frente + 1) % SIZE;  
			if (bit_IRQ)
			{
					enable_irq();
			}
        return 1; // Evento extraído con éxito
    } else {
 
        return 0; // La cola está vacía
    }
    
}

// Función para obtener estadísticas de eventos
uint32_t FIFO_estadisticas(EVENTO_T ID_evento) {
    return numV[ID_evento];
}
