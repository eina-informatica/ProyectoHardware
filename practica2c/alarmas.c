#include "alarmas.h"

void alarma_inicializar(void) {
		int i;
    for (i = 0; i < MAX_ALARM; i++){
        alarmas[i].id = -1;
        alarmas[i].periodica = 0;
        alarmas[i].auxData = 0;
        duracion[i] = 0;
        alarmas[i].activa = 0;
    }
}

void alarma_activar(EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData) {
    uint8_t periodica = (retardo & 0x800000) >> 23;  // bit que indica si la alarma es periódica
    uint32_t nuevo_periodo = retardo & 0x7fffff;  // milisegundos de retardo
    unsigned int encontrada = 0;
    
    // Si ya hay alarmas configuradas se reprograma
		int i;
		for (i = 0 ; i < MAX_ALARM; i++){
				if(alarmas[i].id ==ID_evento){
            encontrada=1;
            if (nuevo_periodo > 0) {
                alarmas[i].id = ID_evento;
                alarmas[i].periodica = periodica;
                alarmas[i].retardo = nuevo_periodo;
								alarmas[i].activa=1;
                break;
            }else{
                alarmas[i].activa = 0;
								duracion[i] = 0;
            }
       }
	
		}
    // Si no hay alarmas configuradas se añade
		if(!encontrada){
				for (i = 0; i < MAX_ALARM; i++) {
						if (alarmas[i].activa == 0 && nuevo_periodo > 0 && !encontrada){
								encontrada=1;
								alarmas[i].id = ID_evento;
								alarmas[i].periodica = periodica;
								alarmas[i].auxData = auxData;
								alarmas[i].retardo = nuevo_periodo;
								alarmas[i].activa=1;
						}
				}
    }
    if(!encontrada && nuevo_periodo > 0){
        FIFO_encolar(ALARMA_OVERFLOW,0);
    }
}
void alarma_tratar_evento(void){
		int i;
    for(i = 0; i < MAX_ALARM; i++){
        if(alarmas[i].activa ==1){
            if(alarmas[i].retardo <= duracion[i]+1){
                FIFO_encolar(alarmas[i].id,alarmas[i].auxData);
                if(alarmas[i].periodica==1){
                    duracion[i]=0;//Reiniciamos el contador
                }else{
                    alarmas[i].activa=0;//Eliminamos la alarma
                }
            }else{
                duracion[i]++;//Cada periodo es 1 ms por lo tanto sumamos de 1 en 1
            } 
        }
    }
}
