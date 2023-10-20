#include <stdint.h>
#include <LPC210X.H>
#include "gpio_hal.h"
#include "cola.h"
// Definición del tipo de datos EVENTO_T


// Estructura para un elemento de la cola
// Inicialización de la cola FIFO
void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow) {
    frente = 0; // lgo sumar ++ eb las iter
    final = 0;
    gpio_hal_escribir(pin_overflow, GPIO_OVERFLOW_BITS, 0);
}

// Función para encolar un evento
void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData) {
    if ((final + 1) % SIZE != frente) {
        cola[final].id =ID_evento;
        cola[final].auxDATA = auxData;
        numV[ID_evento]++;
        final = (final + 1) % SIZE;
    } else {
        gpio_hal_escribir(pin_overflow, GPIO_OVERFLOW_BITS, 1);
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