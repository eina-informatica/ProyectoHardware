#include "linea_serie_drv.h"

//void (*callback_func_temp2)(char);

void gestion_caracter(char c){
    gestion_caracter_hal(c);
}

/*void tuputamdre2(){
    if (callback_func_temp != NULL) {
        callback_func_temp(character);
    }
}*/

void linea_serie_drv_iniciar(/*char c, void (*funcion_tratar_caracter)()*/void){
    /*callback_func_temp = funcion_tratar_caracter;
    character = c;*/
    uart0_init(gestion_caracter);
}

void linea_serie_drv_enviar_array(char cadena[]){
    linea_serie_hal_enviar_array(cadena);
}

void linea_serie_drv_continuar_envio(){
    linea_serie_hal_continuar_envio();
}

