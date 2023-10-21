
#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "timer_hal.h"
#include "pulsacion.h"

// Constant to convert ticks to microseconds
static volatile unsigned int count_timer = 0;
void timer0_ISR (void) __irq;    // Generate Interrupt 
// Function to initialize the timer
void temporizador_hal_iniciar() {
    // Configure the timer
    T0MCR = 3; //bit 0 Interrupt on MR0: an interrupt is generated when MR0 matches the value in the TC
               // bit 2 Stop on MR0: the TC and PC will be stopped and TCR[0] will be set to 0 if MR0 matches the TC.
	T0MR0 = 149999;
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
    if (callback_func != null) {
        callback_func();
    }
}

void temporizador_hal_reloj(uint32_t periodo, void (*funcion_callback)()) {
    // Almacena la función de devolución en la variable global
    if (periodo == 0)
    {
        T1TCR = 2; // When one, the Timer Counter and the Prescale Counter are synchronously reset 
        T1TCR = 0; // Disable the timer
    }else{
        callback_func = funcion_callback;
        // Configure the timer
        T1MCR = 3; //bit 0 Interrupt on MR0: an interrupt is generated when MR0 matches the value in the TC
                    // bit 2 Stop on MR0: the TC and PC will be stopped and TCR[0] will be set to 0 if MR0 matches the TC.
        T1MR0 = periodo*1000;
        VICVectAddr1 = (unsigned long) timer1_ISR; // set interrupt vector in 0
        VICVectCntl1 = 0x20 | 5;  
        //inicializar contador de interrupciones
        T1TCR = 1; // Enable the timer
        VICIntEnable = VICIntEnable | 0x00000011; // Enable Timer0 Interrupt
    }
}

/*

// variable para contabilizar el n�mero de interrupciones
static volatile unsigned int timer0_int_count = 0;

void timer0_ISR (void) __irq;    // Generate Interrupt 


Setup the Timer Counter 0 Interrupt 
void timer0_init (void) {
		timer0_int_count = 0;	
	// configuration of Timer 0
		T0MR0 = 149999;                        // Interrumpe cada 0,05ms = 150.000-1 counts
    T0MCR = 3;                     // Generates an interrupt and resets the count when the value of MR0 is reached
    T0TCR = 1;                             // Timer0 Enable
    // configuration of the IRQ slot number 0 of the VIC for Timer 0 Interrupt
		VICVectAddr0 = (unsigned long)timer0_ISR;          // set interrupt vector in 0
    // 0x20 bit 5 enables vectored IRQs. 
		// 4 is the number of the interrupt assigned. Number 4 is the Timer 0 (see table 40 of the LPC2105 user manual  
		VICVectCntl0 = 0x20 | 4;                   
    VICIntEnable = VICIntEnable | 0x00000010;                  // Enable Timer0 Interrupt
}


 Timer Counter 0 Interrupt executes each 10ms @ 60 MHz CPU Clock 
void timer0_ISR (void) __irq {
    timer0_int_count++;
    T0IR = 1;                              // Clear interrupt flag
    VICVectAddr = 0;                            // Acknowledge Interrupt
}


*/
