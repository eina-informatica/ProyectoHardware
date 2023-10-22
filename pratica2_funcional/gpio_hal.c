#include "gpio_hal.h"

__inline void gpio_hal_iniciar(void){
    PINSEL0=0;
    PINSEL1=0; 
}

__inline void 
gpio_hal_sentido(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, gpio_hal_pin_dir_t direccion){

    if (direccion == GPIO_HAL_PIN_DIR_INPUT) {
            // 0AF00030
        uint32_t aux = ~((~(0xFFFFFFFF << num_bits)) << gpio_inicial);
                IODIR = IODIR & aux;
    } else if (direccion == GPIO_HAL_PIN_DIR_OUTPUT){
        // Configura el pin GPIO como salida
        uint32_t aux = ((~(0xFFFFFFFF << num_bits)) << gpio_inicial);
                IODIR = IODIR | aux;
    }
}
__inline uint32_t gpio_hal_leer(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits){
    return  (IOPIN >> gpio_inicial) & ~(0xFFFFFFFF << num_bits); // desplazamiento de IOPIN por bit_inicial para coloca el bit
                                                            // bit_inicial en la posición menos significativa.
                                                            
    //IO0PIN && 0xFFFF00FF) || 0x0000A500)

}
__inline void gpio_hal_escribir(GPIO_HAL_PIN_T bit_inicial, uint8_t num_bits, uint32_t valor){
    uint32_t valor_negado=~valor;                                                             
    valor = valor & (~(0xFFFFFFFF << num_bits));//ponemos a 0 los bits que queremos escribir y los negamos
    valor= valor << bit_inicial; //desplazamos el valor a la posición que queremos escribir
    //IODIR= IODIR | valor; //ponemos a 1 los bits que queremos escribir
    IOSET= valor; //escribimos el valor
    valor_negado= valor_negado & (~(0xFFFFFFFF<< num_bits));//ponemos a 0 los bits que queremos escribir y los negamos
    valor_negado= valor_negado << bit_inicial; //desplazamos el valor a la posición que queremos escribir
    IOCLR= valor_negado; //escribimos el valor
}


