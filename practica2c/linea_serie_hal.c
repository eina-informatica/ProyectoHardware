#include "linea_serie_hal.h"

static void (*callback_func_temp2)(char);
static volatile enum estado_envio estado;
static volatile enum estado_recibir maq_estado=inicio;
static uint32_t pin,size;
static uint32_t USUARIO_AUSENTE=8400608;//---> 12 seg * 0x800000 para activar periocidad
static char buffer[4];

static void (*callback_fuc)(EVENTO_T, uint32_t);
static void (*callback_fuc_a)(EVENTO_T, uint32_t, uint32_t);

void uart0_init(void (*funcion_callback)(char),void (*callback)(EVENTO_T, uint32_t),uint32_t err_pin,uint32_t err_size,void (*callback_a)(EVENTO_T, uint32_t, uint32_t)){
    PINSEL0 = PINSEL0 | 0x5; // Configuramos los pines P0.0 y P0.1 como TXD0 y RXD0
    U0LCR = 0x83;//Set DLAB=1, Divisor Latch Access Bit y 8 bits de datos y bit de parada
    U0DLL = 97;//Establecemos la tasa de baudios
    U0LCR = 3;//Set DLAB=0,ya que hemos terminado de configurar la tasa de baudios
    U0IER = 3;//Habilitamos las interrupciones por recepción y transmisión
    VICVectCntl4 = 0x20 | 6;//Habilitamos la interrupción UART0
    VICVectAddr4 = (unsigned long)uart0_ISR;
    VICIntEnable  = 0x00000040;
	pin=err_pin;
	size=err_size;
		estado=libre;
    callback_func_temp2=funcion_callback;
	callback_fuc_a=callback_a;	
	callback_fuc=callback;

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

void gestion_caracter_hal(char c){
  static int error=0;
	callback_fuc_a(dormir,USUARIO_AUSENTE,0);
	switch(maq_estado){
		case inicio:
			if (c=='$'){	
				//buffer = (char*)malloc(3 * sizeof(char));
				 //memset(buffer, 0, 3);
				  gpio_hal_escribir(pin,size,0);
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
									callback_fuc(ev_RX_SERIE,buffer[0] << 16 | buffer[1] << 8 | buffer[2]);
									
							}else if(strcmp(buffer,"END")==0){
									//acabar partida
									callback_fuc(ev_RX_SERIE,buffer[0] << 16 | buffer[1] << 8 | buffer[2]);
							
							}else if (buffer[1]=='-'&& isdigit(buffer[2]) && isdigit(buffer[0])){//comprobar que hace bien isdigit
									//mover ficha
									if (tablero_fila_valida(buffer[0]-'0') && tablero_columna_valida(buffer[2]-'0')){
											callback_fuc(ev_RX_SERIE,buffer[0] << 16 | buffer[1] << 8 | buffer[2]);
									}else{
											error=1;
									}
							}else if (strcmp(buffer,"TAB")==0){
										callback_fuc(ev_RX_SERIE,buffer[0] << 16 | buffer[1] << 8 | buffer[2]);
							}
							else{
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
						gpio_hal_escribir(pin,size,1);
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
        callback_fuc(ev_TX_SERIE,0);
    }else {
        U0THR = buffer_envio[indice++];
		U0IER |=2;
    }
}
