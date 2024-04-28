#include "cola.h"

// Cola de ítems
struct ITEM cola[SIZE];

// Vector con el número de veces que se ha encolado un determinado evento
// Se usa con fines estadísticos
int numV[max_Eventos];

// Parámetros de la cola
uint8_t frente;
uint8_t final;
uint8_t i; 

// Datos para escribir en el pin de overflow cuando lo haya
GPIO_HAL_PIN_T gpio_overflow;
uint8_t gpio_overflow_bits;

// Dirección del pin GPIO
gpio_hal_pin_dir_t sent;

// Variable que almacena el estado de las interrupciones
static uint32_t bit_IRQ;

// Estructura para un elemento de la cola
// Inicialización de la cola FIFO
static void (*callback_escribir)(GPIO_HAL_PIN_T, uint8_t, uint32_t);
static void (*callback_sentido)(GPIO_HAL_PIN_T, uint8_t, gpio_hal_pin_dir_t);

// Inicializa la cola FIFO y establece los callbacks para escribir en el pin de desbordamiento y configurar el sentido del pin. 
// Además, inicializa la cola y el vector de estadísticas.
void FIFO_inicializar(GPIO_HAL_PIN_T gpio_of, uint8_t gpio_of_bits, gpio_hal_pin_dir_t sentido, void (*callback_e)(GPIO_HAL_PIN_T, uint8_t, uint32_t), void (*callback_s)(GPIO_HAL_PIN_T, uint8_t, gpio_hal_pin_dir_t)) {
    bit_IRQ = read_irq_bit();
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

// Encola un nuevo evento en la cola, actualizando las estadísticas. 
// Si la cola está llena, se activa el pin de overflow
void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData) {
    bit_IRQ = read_irq_bit();
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

//  Extrae un evento de la cola y devuelve sus datos asociados. Si la cola está vacía, retorna 0.
uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t *auxData) {
    bit_IRQ = read_irq_bit();
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
