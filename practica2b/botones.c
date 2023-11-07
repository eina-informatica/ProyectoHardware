#include "botones.h"

void boton_init(void){
	
    eint1_init();
    eint2_init();
}

/*void boton_encolar(void){
    if(eint1_nueva_pulsacion){
        FIFO_encolar(Eint1, 1);
        eint1_clear_nueva_pulsacion();
        alarma_activar(comprobar,8388708,1);
    }if (eint2_nueva_pulsacion)
    {
        FIFO_encolar(Eint2, 2);
        eint2_clear_nueva_pulsacion();
        alarma_activar(comprobar,8388708,2);
    } 
}*/

void boton_pulsado(uint8_t ID_evento){
    switch(ID_evento){
        case Eint1:
            estado1=UP;
						// 0x800064 --> 0x64 --> 100ms *0x800000
						alarma_activar(comprobar1,cienMS_period,1);
						alarma_activar(dormir, 0, 0);
            break;
        case Eint2:
            estado2=UP;
						// 0x800064 --> 0x64 --> 100ms *0x800000
						alarma_activar(comprobar2,cienMS_period,2);
						alarma_activar(dormir, 0, 0);
            break;
        default:
            break;
    }

}

void boton_comprobar_pulsacion(uint8_t boton){
    switch (boton){
				case 1:
						if(!eint1_sigue_pulsado()){
								estado1=DOWN;
								eint1_habilitar_interrupciones();
								alarma_activar(comprobar1,0,1);
								alarma_activar(dormir,USUARIO_AUSENTE,0);
						}
						break;
				case 2:
						if(!eint2_sigue_pulsado()){
								estado2=DOWN;
								eint2_habilitar_interrupciones();
								alarma_activar(comprobar2,0,2);
								alarma_activar(dormir,USUARIO_AUSENTE,0);
						}
				break;
		}
}
