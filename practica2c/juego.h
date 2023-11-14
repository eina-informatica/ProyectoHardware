//#include <inttypes.h>
#include "cola.h"
#include <inttypes.h>
#include "timer_drv.h"
#include "linea_serie_hal.h"
#include "tablero.h"

#ifndef JUEGO_H
#define JUEGO_H
static uint32_t cuenta;
static uint32_t intervalo;
static uint32_t ant_intervalo;
static char tablero[200];
static CELDA celda;
static uint8_t row = 0;
static uint8_t column = 0;
static uint8_t color = 1;
static unsigned int final = 0;
static unsigned int ganador = 0;
static uint32_t tiempo_ini = 0;
static uint32_t tiempo_fin = 0;


void juego_inicializar(void);
void juego_tratar_evento(EVENTO_T ID_evento, uint32_t auxData);
int juego_leer_cuenta(void);
int juego_leer_intervalo(void);
uint8_t conecta_K_hay_linea(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color);
uint8_t conecta_K_buscar_alineamiento(TABLERO *t, uint8_t fila,
uint8_t columna, uint8_t color, int8_t delta_fila, int8_t delta_columna);
void conecta_K_visualizar_tiempo(uint32_t tiempo);
void conecta_K_visualizar_tablero(CELDA cuadricula[7][8]);
void conecta_K_reiniciar_tablero(void);
void conecta_K__reiniciar_tablero(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

#endif
