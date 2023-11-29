#include <LPC210X.H>
#include "evento.h"

#ifndef INT_EXTERNAS_HAL_H
#define INT_EXTERNAS_HAL_H

void eint1_init(void (*callback_f)(EVENTO_T, uint32_t));

int eint1_read_nueva_pulsacion(void);

void eint1_clear_nueva_pulsacion(void);

int eint1_sigue_pulsado(void);

void eint1_habilitar_interrupciones(void);

void eint2_init(void (*callback_f)(EVENTO_T, uint32_t));

int eint2_read_nueva_pulsacion(void);

void eint2_clear_nueva_pulsacion(void);

int eint2_sigue_pulsado(void);

void eint2_habilitar_interrupciones(void);

#endif
