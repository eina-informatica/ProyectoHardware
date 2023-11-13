#include "visualizar.h"

void inicializar_visualizar(void){
		INI_Visualizar=16;
		Tam_Visualizar=8;
    gpio_hal_sentido(INI_Visualizar,Tam_Visualizar,GPIO_HAL_PIN_DIR_OUTPUT);

}
void visualizar(uint32_t cuenta){
   int count = cuenta & 0xff;//Obtenemos los 8 bits de menor peso
   gpio_hal_escribir(INI_Visualizar,Tam_Visualizar,count);
}
