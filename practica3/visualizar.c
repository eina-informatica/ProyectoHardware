#include "visualizar.h"

uint32_t ini,bits;
gpio_hal_pin_dir_t dir;
void (*callback_escribir)(GPIO_HAL_PIN_T, uint8_t, uint32_t);

// Función para inicializar el módulo de visualización
void inicializar_visualizar(uint32_t gpio_visu, uint32_t gpio_visu_bits, gpio_hal_pin_dir_t direccion,void (*callback_2)(GPIO_HAL_PIN_T, uint8_t, gpio_hal_pin_dir_t),void (*callback)(GPIO_HAL_PIN_T, uint8_t, uint32_t)){
   ini=gpio_visu;
   bits=gpio_visu_bits;
   dir=direccion;
   callback_escribir=callback;
   callback_2(ini,bits,dir);
}

// Función para visualizar los 8 bits de menos peso 
void visualizar(uint32_t cuenta){
   int count = cuenta & 0xff; //Obtenemos los 8 bits de menor peso
   callback_escribir(ini,bits,count);
}
