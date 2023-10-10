#include "timer_hal.h"
/*
void temporizador_hal_iniciar(void){
    // Inicializa el timer

    timer0_int_count = 0;
    T0MCR = 3;
    VICVectCntl0 = 0x20 | 4;
    VICVectAddr0 = (unsigned long) timer0_ISR;

    timer1_int_count = 0;
    T1PR = 2999;  configura el TC para que cuente cada 3000-1 ciclos = 1 us
    T1MR0 = 1000;  interrumpe cada 1000 us = 1 ms
    T1MCR = 3;  reinicia el contador después de cada interrupción
    VICVectCntl1 = 0x20 | 5;
    VICVectAddr1 = (unsigned long) timer1_ISR;  
}

void temporizador_empezar(void) {
    T0TCR = 1;
    VICIntEnable = VICIntEnable | 0x10;
    T1TCR = 1;
    VICIntEnable = VICIntEnable | 0x20;
}

uint32_t temporizador_leer(void) {
    return timer1_int_count * 1000 + T1TC;  //número de interrupciones (una por ms) + tiempo contado en us desde la última interrupción 
}
uint32_t temporizador_parar(void) {
    T1TCR = 3;
    T1TCR = 0;

return temporizador_leer();
}
void temporizador_hal_empezar() {

  
}
uint64_t temporizador_hal_leer(){

}
uint64_t temporizador_hal_parar(){

}
*/

// Constant to convert ticks to microseconds
static volatile unsigned int count_timer = 0;

// Function to initialize the timer
void temporizador_hal_iniciar() {
    // Configure the timer
    VICVectCntl0 = 0x20 | 4;
    //VICVectAddr0 = (unsigned long)timer0_ISR; Determinar la direccion de la interrupcion ????
    T0MCR = 3 //bit 0 Interrupt on MR0: an interrupt is generated when MR0 matches the value in the TC
            // bit 2 Stop on MR0: the TC and PC will be stopped and TCR[0] will be set to 0 if MR0 matches the TC.
    T0PR = CCLK / 1000000 - 1; // Prescale to 1us
    T0MR0 = 3000;  //interrumpe cada 3000 us = 3 ms
    //inicializar contador de interrupciones
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
}

// Function to stop the timer and return the elapsed time in ticks
uint64_t temporizador_hal_parar() {
    T0TCR = 2; // When one, the Timer Counter and the Prescale Counter are synchronously reset 
    T0TCR = 0; // Disable the timer
    return temporizador_hal_leer(); // Return the elapsed time
}