#include "linea_serie_hal.h"

void (*callback_func_temp2)(char);
static volatile enum estado_envio estado;
static volatile enum estado_recibir maq_estado=inicio;

void uart0_init(void (*funcion_callback)()){
    PINSEL0  = 0x5; // Configuramos los pines P0.0 y P0.1 como TXD0 y RXD0
    U0LCR = 0x83;//Set DLAB=1, Divisor Latch Access Bit y 8 bits de datos y bit de parada
    U0DLL = 97;//Establecemos la tasa de baudios
    U0LCR = 3;//Set DLAB=0,ya que hemos terminado de configurar la tasa de baudios
    U0IER = 3;//Habilitamos las interrupciones por recepción y transmisión
    VICVectCntl4 = 0x20 | 6;//Habilitamos la interrupción UART0
    VICVectAddr4 = (unsigned long)uart0_ISR;
    VICIntEnable  = 0x00000040;
		estado=libre;
    callback_func_temp2=funcion_callback;
}
unsigned int uart0_caracter_recibido(void) {
    return (U0IIR & 0xe) == 4;
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
	}else{
		linea_serie_hal_continuar_envio();
	}

}
void gestion_caracter_hal(char c){
    static int error=0;
	switch(maq_estado){
		case inicio:
			if (c=='$'){	
				//buffer = (char*)malloc(3 * sizeof(char));
				 memset(buffer, 0, 3);
				  if(error){gpio_hal_escribir(GPIO_SERIE_ERROR,GPIO_SERIE_ERROR_BITS,0);}
          i=0;
					error=0;
					//ini_array=1;
					maq_estado=durante;
			}
			break;
		case durante:
			 if (c=='!'){
					if (i==3){
                        if (strcmp(buffer,"NEW")==0)
                        {
                            //crear tablero
														FIFO_encolar(ev_RX_SERIE,*buffer);                            
                        }else if(strcmp(buffer,"END")==0){
                            //acabar partida
                            FIFO_encolar(ev_RX_SERIE,*buffer);
                        }else if (buffer[1]=='-'&& isdigit(buffer[2]) && isdigit(buffer[0])){//comprobar que hace bien isdigit
                            //mover ficha
                            FIFO_encolar(ev_RX_SERIE,*buffer);
                        }else{
                            error=1;}
												maq_estado=inicio;
					}else{
							error=1;
					}
			}
			else{
					if (i<3){
							buffer[i]=c;
							i++;
					}
					else{
							error=1;
					}
			}
			break;
		}
        if (error){
					error=0;
					i=0;
					maq_estado=inicio;
					estado=libre;
					gpio_hal_escribir(GPIO_SERIE_ERROR,GPIO_SERIE_ERROR_BITS,1);
			    }
    }




 void linea_serie_hal_enviar_array(char cadena[]){
    indice = 0;
//    buffer_envio = (char*)malloc(200 * sizeof(char));
    memset(buffer_envio, 0, 200);
    strcpy(buffer_envio, cadena);
    U0THR = buffer_envio[indice++];
    estado = ocupado;

}

void linea_serie_hal_continuar_envio(){
    if (buffer_envio[indice] =='\0') {
        estado = libre;
        FIFO_encolar(ev_RX_SERIE,0);
    }else {
        U0THR = buffer_envio[indice++];
    }
}
