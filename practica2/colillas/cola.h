#include "io_reserva.h"

#define SIZE 32
/*Definición del tipo de datos EVENTO_T: Conjunto de eventos posibles. En
el fichero de cabecera se incluirá el tipo de datos y el conjunto de
posibles eventos identificados con nombre humano. Reservemos el ID
VOID con valor cero para inicializar la cola.*/


 /*Inicialización de la
cola. Se le pasa como parámetro el pin del GPIO utilizado para marcar
errores.*/
void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow);
/* Esta función
guardará en la cola el evento. El campo ID_evento, que permita
identificar el evento (p.e. qué interrupción ha saltado) y el campo
auxData en caso de que el evento necesite pasar información extra.*/
void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData);
/*Sihay eventos sin procesar, devuelve un valor distinto de cero y el evento
más antiguo sin procesar por referencia. Cero indicará que la cola está
vacía y no se ha devuelto ningún evento.*/
uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t* auxData);

/*Dado un identificador de evento nos devuelve el número total de veces que ese
evento se ha encolado. El evento VOID nos devolverá el total de eventos
encolados desde el inicio. 
*/
uint32_t FIFO_estadisticas(EVENTO_T ID_evento);

