#include <stdint.h>
#include <LPC210X.H>
#include "cola.h"
// Definición del tipo de datos EVENTO_T
typedef struct
{
    uint8_t ID_evento;
    uint32_t auxData, numveces=0;
} EVENTO_T;

 enum ID_evento {
    Timer0 = 0,
    //se iran añadiendo nuevos
};


// Estructura para un elemento de la cola
// Inicialización de la cola FIFO
void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow) {
    EVENTO_T cola[SIZE];
    uint8_t frente = 0;
    uint8_t final = 0;
    GPIO_HAL_PIN_T pin_overflow=0;
    // Realiza la inicialización de la cola, si es necesario
}

// Función para encolar un evento
void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData) {
    if ((final + 1) % SIZE != frente) {
        cola[final].ID_evento = ID_evento;
        cola[final].auxData = auxData;
        final = (final + 1) % SIZE;
    } else {
        overflow();//GPIO31=1 FALTA
        // La cola está llena, manejo de error o desbordamiento 
    }
}

// Función para extraer un evento de la cola
uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData) {
    if (frente != final) {
        *ID_evento = cola[frente].ID_evento;
        *auxData = cola[frente].auxData;
        frente = (frente + 1) % SIZE;
        return 1;  // Evento extraído con éxito
    } else {
        return 0;  // La cola está vacía
    }
}

// Función para obtener estadísticas de eventos
uint32_t FIFO_estadisticas(EVENTO_T ID_evento) {
    uint32_t contador = 0;
    for (uint8_t i = 0; i < SIZE; i++) {
        if (cola[i].ID_evento == ID_evento) {
            contador++;
        }
    }
    return contador;
}
