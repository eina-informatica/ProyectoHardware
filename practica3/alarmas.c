#include "alarmas.h"

#define MAX_ALARM 4

typedef struct {
    uint8_t periodica;
    uint8_t activa;
    EVENTO_T id;
    uint32_t retardo;
	uint32_t auxData;
} Alarma;

static Alarma alarmas[MAX_ALARM];
//static unsigned int periodo = 10;
static unsigned int duracion[MAX_ALARM];
static void (*callback_func)(EVENTO_T, uint32_t); // FIFO encolar

// Función para inicializar el módulo de alarmas
void alarma_inicializar(void(*funcion_callback)(EVENTO_T, uint32_t)) {
    int i;
    callback_func = funcion_callback;
    for (i = 0; i < MAX_ALARM; i++){
        alarmas[i].id = kk;
        alarmas[i].periodica = 0;
        alarmas[i].auxData = 0;
        duracion[i] = 0;
        alarmas[i].activa = 0;
    }
}

// Función para activar una nueva alarma dado un ID_evento, un retardo y datos auxiliares
void alarma_activar(EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData) {
    uint8_t periodica = (retardo & 0x800000) >> 23;  // bit que indica si la alarma es periódica
    uint32_t nuevo_periodo = retardo & 0x7fffff;  // milisegundos de retardo
    unsigned int encontrada = 0;
    
    // Si ya hay alarmas configuradas se reprograma
    int i;
    for (i = 0 ; i < MAX_ALARM; i++){
        if(alarmas[i].id == ID_evento){
            encontrada = 1;
            if (nuevo_periodo > 0) {
                alarmas[i].id = ID_evento;
                alarmas[i].periodica = periodica;
                alarmas[i].retardo = nuevo_periodo;
                alarmas[i].activa = 1;
				duracion[i] = 0;
                break;
            } else {
                alarmas[i].activa = 0;
                duracion[i] = 0;
            }
        }
    }
    
    // Si no hay alarmas configuradas se añade
    if(!encontrada){
        for (i = 0; i < MAX_ALARM; i++) {
            if (alarmas[i].activa == 0 && nuevo_periodo > 0 && !encontrada){
                encontrada = 1;
                alarmas[i].id = ID_evento;
                alarmas[i].periodica = periodica;
                alarmas[i].auxData = auxData;
                alarmas[i].retardo = nuevo_periodo;
                alarmas[i].activa = 1;
                break;
            }
        }
    }
    
    if(!encontrada && nuevo_periodo > 0){
        callback_func(ALARMA_OVERFLOW, 0); // Call the callback function
    }
}

// Esta función itera por todas las alarmas comprobando cuál está activa, si el tiempo que ha trascurrido 
// desde que se creo la alarma (duracion[i]) es inferior al retardo, incrementamos el tiempo trascurrido.
// En caso de que el tiempo desde que se creó la alarma sea igual o superior al retardo de esta, es hora de 
// atenderla, para ello se encola en la cola FIFO el evento que contiene y sus datos auxiliares
void alarma_tratar_evento() {
    int i;
    for (i = 0; i < MAX_ALARM; i++) {
        if (alarmas[i].activa == 1) {
            if (alarmas[i].retardo <= duracion[i] + 1) {
                callback_func(alarmas[i].id, alarmas[i].auxData); // Call the callback function
                if (alarmas[i].periodica == 1) {
                    duracion[i] = 0; // Reset the counter
                } else {
                    alarmas[i].activa = 0; // Disable the alarm
                }
            } else {
                duracion[i]++; // Cada periodo es 1 ms por lo tanto sumamos de 1 en 1
            }
        }
    }
}
