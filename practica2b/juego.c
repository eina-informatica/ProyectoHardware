#include "juego.h"

void juego_inicializar(){
     cuenta=0;
     intervalo=0;
		 ant_intervalo=0;

}

void juego_tratar_evento(EVENTO_T ID_evento, uint32_t auxData){
    if (ID_evento==Eint1)
    {
        cuenta++;
        FIFO_encolar(ev_VISUALIZAR_CUENTA,cuenta);
        intervalo=temporizador_drv_leer()-ant_intervalo;
				ant_intervalo = temporizador_drv_leer(); 
        
    }else if (ID_evento==Eint2)
    {
        cuenta--;
        FIFO_encolar(ev_VISUALIZAR_CUENTA,cuenta);
        intervalo=temporizador_drv_leer()-ant_intervalo;
				ant_intervalo = temporizador_drv_leer(); 
    }
}

int juego_leer_cuenta(void){
    return cuenta;
}

int juego_leer_intervalo(void){
    return intervalo;
}
