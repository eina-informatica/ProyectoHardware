#include "planificador.h"


//Se encargará de iniciar la cola 
void planificador() {
		uint32_t aux=0;
		EVENTO_T event;
//		static uint32_t bit_IRQ;
		//inicializar();
//		static int USUARIO_AUSENTE=8400608;//---> 12 seg * 0x800000 para activar periocidad
		//int res=1,i,toal,
		temporizador_drv_iniciar();
		temporizador_drv_empezar();
		gpio_hal_sentido(0, 32, GPIO_HAL_PIN_DIR_OUTPUT);
		
		//for (i=0;i<1000000;i++){res*=res;res-=4;}
		//	toal=temporizador_drv_parar(); 	//Pruebas de tiempo
		//gpio_hal_escribir(0, 32, 0);
		gpio_hal_escribir(14,2,3);
		gpio_hal_sentido(14, 2, GPIO_HAL_PIN_DIR_INPUT);
			
		FIFO_inicializar(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS,GPIO_HAL_PIN_DIR_OUTPUT,gpio_hal_escribir,gpio_hal_sentido);
		alarma_inicializar(FIFO_encolar);
		boton_init(FIFO_encolar,alarma_activar);
		//juego_inicializar();
		inicializar_visualizar(INI_Visualizar,Tam_Visualizar,GPIO_HAL_PIN_DIR_OUTPUT,gpio_hal_sentido,gpio_hal_escribir);
		hello_world_inicializar(alarma_activar, GPIO_HELLO_WORLD, GPIO_HELLO_WORLD_BITS, GPIO_HAL_PIN_DIR_OUTPUT,gpio_hal_sentido);
		temporizador_drv_reloj(1, FIFO_encolar, Timer1);
		alarma_activar(dormir,USUARIO_AUSENTE,0);
		linea_serie_drv_iniciar(FIFO_encolar,GPIO_SERIE_ERROR,GPIO_SERIE_ERROR_BITS,alarma_activar);
		juego_inicializar(FIFO_encolar,linea_serie_drv_enviar_array,alarma_activar);
		WD_hal_init(3);
    	while(1){
				WD_hal_feed();
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
										while (1);
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
										hello_world_tratar_evento(FIFO_encolar);
										break;
								case ev_VISUALIZAR_HELLO:
										hello_world_tick_tack(gpio_hal_escribir);
										break;
								case ev_RX_SERIE:
										juego_tratar_evento(ev_RX_SERIE, aux);
										break;
									case ev_TX_SERIE:
										juego_tratar_evento(ev_TX_SERIE, aux);
										break;
								case restart:
										WD_hal_test();
										break;
								case ev_confirmacion:
										juego_tratar_evento(event,0);
								default:
										// Manejar otros casos aquí
										break;
								}
						}else{energia_idle();}//wait();}
		}
		
		
		/*████████╗███████╗░██████╗████████╗░██████╗
			╚══██╔══╝██╔════╝██╔════╝╚══██╔══╝██╔════╝
			░░░██║░░░█████╗░░╚█████╗░░░░██║░░░╚█████╗░
			░░░██║░░░██╔══╝░░░╚═══██╗░░░██║░░░░╚═══██╗
			░░░██║░░░███████╗██████╔╝░░░██║░░░██████╔╝
			░░░╚═╝░░░╚══════╝╚═════╝░░░░╚═╝░░░╚═════╝░*/

		// TEST DE OVERFLOW EN COLA
/*		while (!gpio_hal_leer(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS)) {
           temporizador_drv_reloj(10, FIFO_encolar, Timer1);
           hello_world_tick_tack(gpio_hal_escribir);
       }*/
		
		
		// TEST DE OVERFLOW EN ALARMAS
		/*while(1){
						alarma_activar(1,1,0);
						alarma_activar(2,1,0);
						alarma_activar(3,1,0);
						alarma_activar(4,1,0);
						alarma_activar(5,1,0);
						if (FIFO_extraer(&event, &aux)) {
								switch (event) {
								case ALARMA_OVERFLOW:
										gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 1); //Alarm_Overflow();
										while (1);
								break;
								default:
									break;
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

		// TEST DE ESCRIBIR UNA PALABRA MAL POR UART
//		 while(1){
//			WD_hal_feed();
//			if (!gpio_hal_leer(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS)) {
//				bit_IRQ=read_irq_bit();
//				if (bit_IRQ)
//				{
//					disable_irq();
//				}
//				if (FIFO_extraer(&event, &aux)) {
//					if(bit_IRQ)
//					{
//						enable_irq();
//					}
//				switch (event) {
//				case ev_RX_SERIE:
//					alarma_activar(dormir,0,0);
//					juego_tratar_evento(ev_RX_SERIE, aux);
//					break;
//				case ev_TX_SERIE:
//					juego_tratar_evento(ev_TX_SERIE, aux);
//					alarma_activar(dormir,USUARIO_AUSENTE,0);
//					break;
//				case restart:
//					WD_hal_test();
//					break;
//				default:
//					// Manejar otros casos aquí
//					break;
//			}
//		}
//	}
//}

		// TEST DE FILA ERRÓNEA
		/*while(1){
			WD_hal_feed();
			if (!gpio_hal_leer(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS)) {
				if (FIFO_extraer(&event, &aux)) {
					switch (event) {
					case ev_RX_SERIE:
						alarma_activar(dormir,0,0);
						juego_tratar_evento(ev_RX_SERIE, aux);
						break;
					case ev_TX_SERIE:
						juego_tratar_evento(ev_TX_SERIE, aux);
						alarma_activar(dormir,USUARIO_AUSENTE,0);
						WD_hal_test();
						break;
					default:
						// Manejar otros casos aquí
						break;
					}
				}
			}
		}*/
}
