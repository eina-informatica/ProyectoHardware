#include "linea_serie_drv.h"

//void (*callback_func_temp2)(char);

// Error en línea serie (más de 3 carácteres recibidos)
uint32_t ser_error_pin, ser_error_size;
// Error en línea serie (comando no válido)
uint32_t com_error_pin, com_error_size;

static char buffer[4];

enum estado_recibir{
	inicio,
	durante
};

static volatile enum estado_recibir maq_estado = inicio;

static void (*callback_func_a)(EVENTO_T, uint32_t, uint32_t);
static void (*callback_func)(EVENTO_T, uint32_t); // Encolar evento
static uint32_t USUARIO_AUSENTE=8400608;//---> 12 seg * 0x800000 para activar periocidad

static int i;

// Función para inicializar el driver de línea serie
void linea_serie_drv_iniciar(void (*callback)(EVENTO_T, uint32_t), uint32_t ser_err_pin, uint32_t ser_err_size, uint32_t com_err_pin, uint32_t com_err_size, void (*callback_a)(EVENTO_T, uint32_t, uint32_t)){
	ser_error_pin = ser_err_pin;
	ser_error_size = ser_err_size;
	com_error_pin = com_err_pin;
	com_error_size = com_err_size;
	callback_func_a = callback_a;
	callback_func = callback;
	i = 0;
  uart0_init(gestion_caracter, callback);
}

// Enviar una cadena de carácteres por línea serie
void linea_serie_drv_enviar_array(char cadena[]){
    linea_serie_hal_enviar_array(cadena);
}

// Función para continuar el envío por línea serie
void linea_serie_drv_continuar_envio(){
    linea_serie_hal_continuar_envio();
}

// Función para gestionar un carácter que llega
void gestion_caracter(char c){
  static int error=0;
	callback_func_a(dormir, USUARIO_AUSENTE, 0);
	switch(maq_estado){
		case inicio:
			if (c=='$'){	
				//buffer = (char*)malloc(3 * sizeof(char));
				 //memset(buffer, 0, 3);
				  gpio_hal_escribir(ser_error_pin, ser_error_size, 0);
					gpio_hal_escribir(com_error_pin, com_error_size, 0);
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
									callback_func(ev_RX_SERIE,buffer[0] << 16 | buffer[1] << 8 | buffer[2]);
									
							}else if(strcmp(buffer,"END")==0){
									//acabar partida
									callback_func(ev_RX_SERIE,buffer[0] << 16 | buffer[1] << 8 | buffer[2]);
							
							}else if (buffer[1]=='-' /*&& isdigit(buffer[2]) && isdigit(buffer[0])*/){//comprobar que hace bien isdigit
									//mover ficha
									if (!(tablero_fila_valida(buffer[0]-'0') || tablero_columna_valida(buffer[2]-'0'))){
										// Poner a 1 el pin 29
										gpio_hal_escribir(com_error_pin, com_error_size, 1);
										error=1;
									}
									/*if (!tablero_fila_valida((buffer[0]-'0')-1)) {
										// Poner a 1 el pin 29
										gpio_hal_escribir(com_error_pin, com_error_size, 1);
										error = 1;
									} else if (!tablero_columna_valida((buffer[2]-'0')-1)) {
										// Poner a 1 el pin 29
										gpio_hal_escribir(com_error_pin, com_error_size, 1);
										error = 1;
									}*/
									callback_func(ev_RX_SERIE, buffer[0] << 16 | buffer[1] << 8 | buffer[2]);
							}else if (strcmp(buffer,"TAB")==0){
										callback_func(ev_RX_SERIE,buffer[0] << 16 | buffer[1] << 8 | buffer[2]);
							}
							else{
									error=1;
								callback_func(ev_RX_SERIE,buffer[0] << 16 | buffer[1] << 8 | buffer[2]);
							}
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
						// Poner a 1 el pin 30
						gpio_hal_escribir(ser_error_pin, ser_error_size, 1);
					}
			}
			break;
		}
    if (error){
				error=0;
				i=0;
				maq_estado=inicio;
				gpio_hal_escribir(com_error_pin, com_error_size, 1);
				//estado=libre;
		}
}
