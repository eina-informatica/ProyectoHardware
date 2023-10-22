/*
Vamos a crear un fichero de cabecera que centralice la reserva de GPIOs. En
este fichero de cabecera (io_reserva.h) se asignar�n los GPIO a un nombre
humano y una longitud que posteriormente utilizar�n los otros m�dulos. 
*/

#ifndef IO_RESERVA_H
#define IO_RESERVA_H

#define GPIO_OVERFLOW  31
#define GPIO_OVERFLOW_BITS 1

#define GPIO_HELLO_WORLD 0
#define GPIO_HELLO_WORLD_BITS 8

#endif
