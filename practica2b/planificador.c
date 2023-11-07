#include "planificador.h"

int stat;


//Se encargará de iniciar la cola 
void planificador() {
		uint32_t aux=0;
		EVENTO_T event;
		//inicializar();
		USUARIO_AUSENTE=8400608;//---> 12 seg * 0x800000 para activar periocidad
		//int res=1,i,toal,
		temporizador_drv_iniciar();
		temporizador_drv_empezar();
		gpio_hal_sentido(0, 32, GPIO_HAL_PIN_DIR_OUTPUT);
		
		//for (i=0;i<1000000;i++){res*=res;res-=4;}
		//	toal=temporizador_drv_parar(); 	//Pruebas de tiempo
		//gpio_hal_escribir(0, 32, 0);
		gpio_hal_escribir(14,2,3);
		gpio_hal_sentido(14, 2, GPIO_HAL_PIN_DIR_INPUT);
	
    FIFO_inicializar(GPIO_OVERFLOW);
	  alarma_inicializar();
		boton_init();
		juego_inicializar();
		inicializar_visualizar();
    hello_world_inicializar();
		temporizador_drv_reloj(1, FIFO_encolar, Timer1);
		alarma_activar(dormir,USUARIO_AUSENTE,0);

    while(1){
				if (!gpio_hal_leer(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS)) {
						if (FIFO_extraer(&event, &aux)) {
								switch (event) {
								case Timer1:
										//hello_world_tick_tack();
										alarma_tratar_evento();
										break;
								case Timer0:
										break;
								case ALARMA_OVERFLOW:
										gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 1); //Alarm_Overflow();
										break;
								case Eint1:
										boton_pulsado(event);
										alarma_activar(dormir,0,0);
										juego_tratar_evento(Eint1, aux);
										break;
								case Eint2:
										boton_pulsado(event);
										alarma_activar(dormir,0,0);
										juego_tratar_evento(Eint2, aux);
										break;
								case comprobar1:
										boton_comprobar_pulsacion(1);
										break;
								case comprobar2:
										boton_comprobar_pulsacion(2);
										break;
								case dormir:
										energia_power_down();
										break;
								case ev_VISUALIZAR_CUENTA:
										visualizar(aux);
										break;
								case ev_LATIDO:
										hello_world_tratar_evento();
										break;
								case ev_VISUALIZAR_HELLO:
										hello_world_tick_tack();
										break;
								default:
										// Manejar otros casos aquí
										break;
								}
						}else{energia_idle();}//wait();}
				}
		}
		
		
		/*████████╗███████╗░██████╗████████╗░██████╗
			╚══██╔══╝██╔════╝██╔════╝╚══██╔══╝██╔════╝
			░░░██║░░░█████╗░░╚█████╗░░░░██║░░░╚█████╗░
			░░░██║░░░██╔══╝░░░╚═══██╗░░░██║░░░░╚═══██╗
			░░░██║░░░███████╗██████╔╝░░░██║░░░██████╔╝
			░░░╚═╝░░░╚══════╝╚═════╝░░░░╚═╝░░░╚═════╝░*/

		// TEST DE OVERFLOW EN COLA
		/*while (!gpio_hal_leer(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS)) {
           temporizador_drv_reloj(10, FIFO_encolar, Timer1);
           hello_world_tick_tack();
       }
       gpio_hal_escribir(GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS, 0);*/
		
		// TEST DE OVERFLOW EN ALARMAS
		/*while(1){
				if (!gpio_hal_leer(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS)) {
						alarma_activar(1,1,0);
						alarma_activar(2,1,0);
						alarma_activar(3,1,0);
						alarma_activar(4,1,0);
						alarma_activar(5,1,0);
						if (FIFO_extraer(&event, &aux)) {
								switch (event) {
								case ALARMA_OVERFLOW:
										gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 1); //Alarm_Overflow();
										break;
								default:
									break;
								}
						}
				}
		}*/
		
		// TEST DE BOTONES
		/*while(1){
				if (!gpio_hal_leer(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS)) {
						if (FIFO_extraer(&event, &aux)) {
								switch (event) {
										case Eint1:
												boton_pulsado(event);
												juego_tratar_evento(Eint1, aux);
										case Eint2:
												boton_pulsado(event);
												juego_tratar_evento(Eint2, aux);
										default:
												// Manejar otros casos aquí
												break;
								}
						}
				}
		}*/
		
		// TEST DE HELLOWORLD
		/*while(1){
				if (!gpio_hal_leer(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS)) {
						if (FIFO_extraer(&event, &aux)) {
								switch (event) {
								case Timer1:
										//hello_world_tick_tack();
										alarma_tratar_evento();
										break;
								case ev_LATIDO:
										hello_world_tratar_evento();
										break;
								case ev_VISUALIZAR_HELLO:
										hello_world_tick_tack();
										break;
								default:
										break;
								}
						}
				}
		}*/
}
