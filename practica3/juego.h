#include "timer_drv.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "tablero.h"

#ifndef JUEGO_H
#define JUEGO_H

void juego_inicializar(void (*callback)(EVENTO_T, uint32_t), void (*callback2)(char*), void (*callback3)(EVENTO_T, uint32_t, uint32_t));
void juego_tratar_evento(EVENTO_T ID_evento, uint32_t auxData);
int juego_leer_cuenta(void);
int juego_leer_intervalo(void);
uint8_t conecta_K_hay_linea(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color);
uint8_t conecta_K_buscar_alineamiento(TABLERO *t, uint8_t fila,
uint8_t columna, uint8_t color, int8_t delta_fila, int8_t delta_columna);
void conecta_K_visualizar_tiempo(uint32_t tiempo);
void conecta_K_visualizar_tablero(int player);
void conecta_K_reiniciar_tablero(void);
void visualizar_jugada(uint32_t fila, uint32_t columna);
char* itoa(int value, char* buffer, int base);
void swap(char *x, char *y);
char* reverse(char *buffer, int i, int j);
void uint32_to_char(uint32_t num, char* str);
//void conecta_K_reiniciar_tablero(TABLERO cuadricula);

#endif
