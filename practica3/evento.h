#include <stdint.h>

#ifndef EVENTO_H
#define EVENTO_H
#define max_Eventos 16
#define MAX_ALARM 4
/*Definici�n del tipo de datos EVENTO_T:*/
enum Evento {
    Timer0,
    Timer1,
    ALARMA_OVERFLOW,
    Eint1,
    Eint2,
    comprobar1,
		comprobar2,
    dormir,
    ev_VISUALIZAR_CUENTA,
    ev_LATIDO,
    ev_VISUALIZAR_HELLO,
    ev_RX_SERIE,
		ev_TX_SERIE,
    restart,
    ev_confirmacion,
    kk=-1
};

typedef enum Evento EVENTO_T;
//typedef uint8_t EVENTO_T;

struct ITEM
{
    EVENTO_T id;
    uint32_t auxDATA;
};

#endif
