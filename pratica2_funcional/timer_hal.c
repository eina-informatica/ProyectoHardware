
#include "timer_hal.h"

// Constant to convert ticks to microseconds
static volatile unsigned int count_timer = 0;
void timer0_ISR (void) __irq;    // Generate Interrupt 
// Function to initialize the timer
void temporizador_hal_iniciar() {
    // Configure the timer
    T0MCR = 3; //bit 0 Interrupt on MR0: an interrupt is generated when MR0 matches the value in the TC
               // bit 2 Stop on MR0: the TC and PC will be stopped and TCR[0] will be set to 0 if MR0 matches the TC.
		T0MR0 = 14999;
    VICVectAddr0 = (unsigned long) timer0_ISR;          // set interrupt vector in 0
    VICVectCntl0 = 0x20 | 4;
    //inicializar contador de interrupciones
    VICIntEnable = VICIntEnable | 0x00000010;                  // Enable Timer0 Interrupt
    count_timer = 0;
}

// Function to start the timer
void temporizador_hal_empezar() {
    T0TCR = 1; // Enable the timer
    VICIntEnable = VICIntEnable | 0x10; // The bit 4 enables de TIMER0 IRQ
}

// Function to read the timer value in ticks
uint64_t temporizador_hal_leer() {
    return (count_timer*3000) + T0TC; // num de interrup * lo q tarda cada intrrup + lo que lleva el Tc de este instante
		//return count_timer;
}


// Function to stop the timer and return the elapsed time in ticks
uint64_t temporizador_hal_parar() {
    T0TCR = 2; // When one, the Timer Counter and the Prescale Counter are synchronously reset 
    T0TCR = 0; // Disable the timer
    return temporizador_hal_leer(); // Return the elapsed time
}
void timer0_ISR (void) __irq {
    count_timer++;
    T0IR = 1; // Clear interrupt flag
    VICVectAddr = 0; // Acknowledge Interrupt
}
unsigned int timer0_read_int_count(void){
	return count_timer;
};

// Timer 1
void (*callback_func)(void);  // Variable global para almacenar la función de devolución

void timer1_ISR() __irq {
    // Llamamos a la función si no es nula
    if (callback_func != NULL) {
        callback_func();
    }
		T1IR = 1; // Clear interrupt flag
    VICVectAddr = 0; // Acknowledge Interrupt
}

void temporizador_hal_reloj(uint32_t periodo, void (*funcion_callback)()) {
  // Almacena la función de devolución en la variable global
  if (periodo==0)
  {
    T1TCR = 2; // When one, the Timer Counter and the Prescale Counter are synchronously reset 
    T1TCR = 0; // Disable the timer
  }else{
			callback_func = funcion_callback;
      T1MCR = 3; //bit 0 Interrupt on MR0: an interrupt is generated when MR0 matches the value in the TC
        // bit 2 Stop on MR0: the TC and PC will be stopped and TCR[0] will be set to 0 if MR0 matches the TC.
      T1MR0 = periodo*3000;
      VICVectAddr1 = (unsigned long) timer1_ISR; // set interrupt vector in 0
      VICVectCntl1 = 0x20 | 5;  
      //inicializar contador de interrupciones
      T1TCR = 1; // Enable the timer
      VICIntEnable = VICIntEnable | 0x20; // Enable Timer0 Interrupt
  }
}
