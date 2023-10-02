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