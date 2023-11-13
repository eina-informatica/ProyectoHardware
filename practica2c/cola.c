#include "cola.h"

struct ITEM cola[SIZE];
int numV[max_Eventos];
uint8_t frente;
uint8_t final;
uint8_t i; 

// Estructura para un elemento de la cola
// Inicialización de la cola FIFO
void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow) {
    frente = 0; // lgo sumar ++ eb las iter
    final = 0;
    gpio_hal_escribir(pin_overflow, GPIO_OVERFLOW_BITS, 0);
		for (i = 0; i < max_Eventos; i++) {
			numV[i] = 0;
		}
}

// Función para encolar un evento
void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData) {
    if ((final + 1) % SIZE != frente) {
        cola[final].id =ID_evento;
        cola[final].auxDATA = auxData;
        numV[ID_evento]++;
        final = (final + 1) % SIZE;
    } else {
				gpio_hal_sentido(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, GPIO_HAL_PIN_DIR_OUTPUT);
        gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 1);
        // La cola está llena, manejo de error o desbordamiento 
    }
}

// Función para extraer un evento de la cola
uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData) {
    if (frente != final) {
        *ID_evento = cola[frente].id;
        *auxData = cola[frente].auxDATA;
        frente = (frente + 1) % SIZE;      
        return 1; // Evento extraído con éxito
    } else {
        return 0; // La cola está vacía
    }
}

// Función para obtener estadísticas de eventos
uint32_t FIFO_estadisticas(EVENTO_T ID_evento) {
    return numV[ID_evento];
}
