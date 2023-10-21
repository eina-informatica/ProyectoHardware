#include <stdint.h>
#include "gpio_hal.h"
#include "io_reserva.h"


#define SIZE 32
#define max_Eventos 10
/*Definici�n del tipo de datos EVENTO_T: Conjunto de eventos posibles. En
el fichero de cabecera se incluir� el tipo de datos y el conjunto de
posibles eventos identificados con nombre humano. Reservemos el ID
VOID con valor cero para inicializar la cola.*/

/*enum EVENTO_T {
    Timer0
    //se iran a�adiendo nuevos
};
*/
typedef uint8_t EVENTO_T;
struct ITEM
{
    EVENTO_T id;
    uint32_t auxDATA;
};

 /*Inicializaci�n de la
cola. Se le pasa como par�metro el pin del GPIO utilizado para marcar
errores.*/
void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow);
/* Esta funci�n
guardar� en la cola el evento. El campo ID_evento, que permita
identificar el evento (p.e. qu� interrupci�n ha saltado) y el campo
auxData en caso de que el evento necesite pasar informaci�n extra.*/
void FIFO_encolar(EVENTO_T ID_evento, uint32_t auxData);
/*Si hay eventos sin procesar, devuelve un valor distinto de cero y el evento
m�s antiguo sin procesar por referencia. Cero indicar� que la cola est�
vac�a y no se ha devuelto ning�n evento.*/
uint8_t FIFO_extraer(EVENTO_T *ID_evento, uint32_t* auxData);

/*Dado un identificador de evento nos devuelve el n�mero total de veces que ese
evento se ha encolado. El evento VOID nos devolver� el total de eventos
encolados desde el inicio. 
*/
uint32_t FIFO_estadisticas(EVENTO_T ID_evento);

