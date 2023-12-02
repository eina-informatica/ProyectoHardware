#include "linea_serie_hal.h"

static void (*callback_func_temp2)(char); // Gestionar carácter
static void (*callback_func)(EVENTO_T, uint32_t); // Encolar evento
static volatile enum estado_envio estado;

static int indice=0;

static char buffer_envio[200];

enum estado_envio {
	ocupado,
	libre
};

void uart0_init(void (*funcion_callback)(char), void (*callback)(EVENTO_T, uint32_t)){
    PINSEL0 = PINSEL0 | 0x5; // Configuramos los pines P0.0 y P0.1 como TXD0 y RXD0
    U0LCR = 0x83;//Set DLAB=1, Divisor Latch Access Bit y 8 bits de datos y bit de parada
    U0DLL = 97;//Establecemos la tasa de baudios
    U0LCR = 3;//Set DLAB=0,ya que hemos terminado de configurar la tasa de baudios
    U0IER = 3;//Habilitamos las interrupciones por recepción y transmisión
    VICVectCntl4 = 0x20 | 6;//Habilitamos la interrupción UART0
    VICVectAddr4 = (unsigned long)uart0_ISR;
    VICIntEnable  = 0x00000040;
	estado=libre;
    callback_func_temp2=funcion_callback;
	callback_func=callback;
}


void uart0_ISR(void) __irq{
    VICVectAddr=0;
		estado=ocupado;
	if((U0IIR & 0x4) == 4){
    if (callback_func_temp2 != NULL)
    {    
        char c=U0RBR;
        callback_func_temp2(c);
    }
	}else /*if ((U0IIR & 0x2)==2)*/{
		linea_serie_hal_continuar_envio();
	}
}

 void linea_serie_hal_enviar_array(char cadena[]){
		int i=0;   
		
	 indice = 0;
		
	 
//    buffer_envio = (char*)malloc(200 * sizeof(char));
	
	while (cadena[i] != '\0') {
		buffer_envio[i] = cadena[i];
		i++;
	}
	buffer_envio[i] = '\0';
//     memset(buffer_envio, 0, 300);
//     strcpy(buffer_envio, cadena);
    U0THR = buffer_envio[indice++];
    estado = ocupado;

}

void linea_serie_hal_continuar_envio(){
    if (buffer_envio[indice] == '\0') {
        estado = libre;
        callback_func(ev_TX_SERIE,0);
    } else {
        U0THR = buffer_envio[indice++];
		U0IER |=2;
    }
}
