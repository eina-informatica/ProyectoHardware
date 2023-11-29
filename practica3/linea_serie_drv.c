#include "linea_serie_drv.h"

//void (*callback_func_temp2)(char);

void gestion_caracter(char c){
    gestion_caracter_hal(c);
}


void linea_serie_drv_iniciar(void (*callback)(EVENTO_T, uint32_t), uint32_t err_pin, uint32_t err_size, void (*callback_a)(EVENTO_T, uint32_t, uint32_t)){
    /*callback_func_temp = funcion_tratar_caracter;
    character = c;*/
    uart0_init(gestion_caracter, callback, err_pin, err_size, callback_a);
}

void linea_serie_drv_enviar_array(char cadena[]){
    linea_serie_hal_enviar_array(cadena);
}

void linea_serie_drv_continuar_envio(){
    linea_serie_hal_continuar_envio();
}

