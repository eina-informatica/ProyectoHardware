#include "timer_hal.h"

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

void temporizador_hal_empezar() {

  
}
uint64_t temporizador_hal_leer(){

}
uint64_t temporizador_hal_parar(){

}