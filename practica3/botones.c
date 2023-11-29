#include "botones.h"

static uint32_t cienMS_period=8388708; // Agrega un punto y coma al final de la declaraci�n.

/* Creamos maquina de estados como un enum de 2 estados */

typedef enum estadoBoton {
    DOWN=0,
    UP=1
}estadoBoton;


estadoBoton estado1; // Asigna los valores iniciales DOWN a las variables estado1 y estado2.
estadoBoton estado2;
static void (*callback_fuc)(EVENTO_T, uint32_t, uint32_t);

void boton_init(void (*callback_f)(EVENTO_T, uint32_t),void (*callback_a)(EVENTO_T, uint32_t, uint32_t)){
	callback_fuc=callback_a;
    eint1_init(callback_f);
    eint2_init(callback_f);
		estado1=DOWN;
		estado2=DOWN;
}



void boton_pulsado(uint8_t ID_evento){
    switch(ID_evento){
        case Eint1:
            estado1=UP;
						// 0x800064 --> 0x64 --> 100ms *0x800000
						callback_fuc(comprobar1,cienMS_period,1);
						//callback_fuc(dormir, 0, 0);
            break;
        case Eint2:
            estado2=UP;
						// 0x800064 --> 0x64 --> 100ms *0x800000
						callback_fuc(comprobar2,cienMS_period,2);
						//callback_fuc(dormir, 0, 0);
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
								callback_fuc(comprobar1,0,1);
								callback_fuc(dormir,USUARIO_AUSENTE,0);
						}
						break;
				case 2:
						if(!eint2_sigue_pulsado()){
								estado2=DOWN;
								eint2_habilitar_interrupciones();
								callback_fuc(comprobar2,0,2);
								callback_fuc(dormir,USUARIO_AUSENTE,0);
						}
				break;
		}
}

