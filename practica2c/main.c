#include "planificador.h"

// Nota: wait es una espera activa. Se puede eliminar poniendo el procesador en modo iddle. Probad a hacerlo
/*void wait (void)  { // wait function
	unsigned int i;

	i = timer0_read_int_count(); // reads the number of previous timer IRQs
	while ((i + 10) != timer0_read_int_count());  // waits for 10 interrupts, i.e. 50ms
}*/

int main (void) {
	//planificador();
	
	FIFO_inicializar(GPIO_OVERFLOW);
	FIFO_encolar(0, 0);
	
	//linea_serie_drv_enviar_array("test");
	//conecta_K_visualizar_tablero();
	while(1);
		
	/*eint0_init(); // activates EINT0 interrupts
	// Nota la gesti�n del GPIO vosotros la debe�s hacer en GPIO.c no en el main o en el reversi
	//IODIR 		= 0x00FF0000;					//Set LED pins as outputs 
	gpio_hal_sentido(16, 8,GPIO_HAL_PIN_DIR_OUTPUT);
	IOCLR = 0x00FF0000;					//Initialices the outputs to 0

	// bucle para comprobar el funcionamiento del bot�n. El objetivo es comprobar que se lleva bien la cuenta de pulsaciones
	// con s�lo una interrupci�n EXTINT0 por pulsaci�n
	// en este proyecto no va a funcionar porque la interrupci�n se activa por nivel y no se ha a�adido la gesti�n necesaria para ue s�lo interrumpa una vez.
	while (eint0_read_count() < 4){
		PM_power_down(); // de aqu� s�lo despertamos si hay pulsaci�n
	};	
	// bucle que realiza un blink de leds cada 50ms	   
	temporizador_hal_iniciar(); // generates an interrupt every 0,05ms and increments timeval0
	temporizador_hal_empezar();
	
	//PRUEBAS	

	while (1)  {  
		int pin16=16;
		for (pin16 = 16; pin16 < 24; pin16++){
			gpio_hal_escribir(pin16,1,1);
			wait();
			gpio_hal_escribir(pin16,1,0);
		}	
		for(pin16=23; pin16 >= 16; pin16--){
			gpio_hal_escribir(pin16,1,1);
			wait();
			gpio_hal_escribir(pin16,1,0);
		}
		total = temporizador_hal_parar();
			
	}

	unsigned int j; // LED var
	unsigned int total;*/

//while (1){
		/* Loop forever */
//    for (j = 0x010000; j < 0x800000; j <<= 1) { /* Blink LED 0,1,2,3,4,5,6 */
      // Nota la gesti�n del GPIO vosotros la debe�s hacer en GPIO.c no en el main o en el reversi
//			IOSET = j;                               /* Turn on LED */
//      wait ();                                  /* call wait function */
//      IOCLR = j;                               /* Turn off LED */
}
 //   for (j = 0x800000; j > 0x010000; j >>=1 ) { /* Blink LED 7,6,5,4,3,2,1 */
      // Nota la gesti�n del GPIO vosotros la debe�s hacer en GPIO.c no en el main o en el reversi
//			IOSET = j;                               /* Turn on LED */
 //     wait ();                                  /* call wait function */
//     IOCLR = j;                               /* Turn off LED */
 //   }
//}

	//	total=temporizador_hal_parar();
		
 // }



//int main (void) {
//  	
//  reversi8();
//	
//}
