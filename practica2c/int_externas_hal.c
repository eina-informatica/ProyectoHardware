#include "int_externas_hal.h"

// Variables globales
static volatile int eint1_nueva_pulsacion = 0;
static volatile int eint2_nueva_pulsacion = 0;
// Declaración de la función de interrupción
void eint1_ISR(void) __irq;
void eint2_ISR(void) __irq;

// Inicialización de la interrupción externa 1
void eint1_init(void) {
    // Reiniciar la bandera de nueva pulsación
		
    eint1_nueva_pulsacion = 0;
		EXTWAKE |=2;
    // Configurar la interrupción externa 1
    EXTINT = EXTINT | 2;
	// Asignar la dirección de la función de interrupción a la entrada 2 del Vector de Interrupciones
    VICVectAddr2 = (unsigned long)eint1_ISR;
    // Configurar el registro de selección de pines para habilitar la función específica
    //haces and con 1 a todos para no modificar los demas pines, y luego or con 1 en el pin 29 para habilitar la funcion EINT1
    PINSEL0 = PINSEL0 & 0xcfffffff;
    PINSEL0 = PINSEL0 | 0x20000000;

    // Configurar la entrada en el Vector Control
    VICVectCntl2 = 0x20 | 15;

    // Habilitar la interrupción en el controlador de interrupciones
   VICIntEnable = VICIntEnable | 0x00008000;

    
}
// Inicialización de la interrupción externa 1
void eint2_init(void) {
    // Reiniciar la bandera de nueva pulsación
    eint2_nueva_pulsacion = 0;
		EXTWAKE |=4;
    // Configurar la interrupción externa 1
    EXTINT = EXTINT | 4;
	VICVectAddr3 = (unsigned long)eint2_ISR;

    // Configurar el registro de selección de pines para habilitar la función específica
    //haces and con 1 a todos para no modificar los demas pines, y luego or con 1 en el pin 31 para habilitar la funcion EINT2
  PINSEL0 = PINSEL0 | 0x80000000;  
	PINSEL0 = PINSEL0 & 0xbfffffff;
    

    // Configurar la entrada en el Vector Control
    VICVectCntl3 = 0x20 | 16;

    // Habilitar la interrupción en el controlador de interrupciones
    VICIntEnable = VICIntEnable | 0x00010000;

    // Asignar la dirección de la función de interrupción a la entrada 2 del Vector de Interrupciones
    
}


// Obtener el estado de nueva pulsación
int eint1_read_nueva_pulsacion(void){
    return eint1_nueva_pulsacion;
}
int eint2_read_nueva_pulsacion(void){
    return eint2_nueva_pulsacion;
}

// Limpiar la bandera de nueva pulsación
void eint1_clear_nueva_pulsacion(void){
    eint1_nueva_pulsacion = 0;
}
void eint2_clear_nueva_pulsacion(void){
    eint2_nueva_pulsacion = 0;
}

// Verificar si la interrupción externa 1 sigue pulsada
int eint1_sigue_pulsado(void) {
    return ((IOPIN & (1<<14))==0);
}
int eint2_sigue_pulsado(void) {
    return ((IOPIN & (1<<15))==0);   
}

// Habilitar las interrupciones de la interrupción externa 1
void eint1_habilitar_interrupciones(void) {
	if((IOPIN & (1<<14))!=0){
		   // Limpiar la flag de la interrupción externa 1
    EXTINT = EXTINT | 2;
		eint1_nueva_pulsacion=0;
    VICIntEnable = VICIntEnable | 0x00008000;
		PINSEL0 = PINSEL0 | 0x20000000;
		PINSEL0 = PINSEL0 &0xFfFFFFFF;
	}
		
}

void eint2_habilitar_interrupciones(void) {
    if((IOPIN & (1<<15))!=0){
		   // Limpiar la flag de la interrupción externa 1
    EXTINT = EXTINT | 4;
			eint2_nueva_pulsacion=0;
    VICIntEnable = VICIntEnable | 0x00010000;
	  PINSEL0 = PINSEL0 | 0x80000000;
		PINSEL0 = PINSEL0 & 0xBfFFFFFF;
	}
}

// Función de interrupción externa 1
void eint1_ISR(void) __irq {
	if(eint1_nueva_pulsacion ==0){
    // Llamar a una función (cola_encolar_eventos) para encolar eventos
		FIFO_encolar(Eint1, 1);

    // Limpiar la bandera de interrupción en el VIC
    VICIntEnClr = 0x00008000;
		PINSEL0 &= 0xCfffffff;

    // Restablecer el Vector Address en el VIC
    VICVectAddr = 0;

    // Indicar que hay una nueva pulsación
    eint1_nueva_pulsacion = 1;
	}
}


void eint2_ISR(void) __irq {
	if(eint2_nueva_pulsacion == 0){
    // Llamar a una función (cola_encolar_eventos) para encolar eventos
    FIFO_encolar(Eint2, 2);

    // Limpiar la bandera de interrupción en el VIC
    VICIntEnClr = 0x00010000;
		PINSEL0 &= 0x3fffffff;


    // Restablecer el Vector Address en el VIC
    VICVectAddr = 0;

    // Indicar que hay una nueva pulsación
    eint2_nueva_pulsacion = 1;
	}
}
	
