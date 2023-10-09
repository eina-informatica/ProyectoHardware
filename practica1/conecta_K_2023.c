#include "conecta_K_2023.h"
#include "entrada.h"
#include <stdio.h>

extern uint8_t conecta_K_buscar_alineamiento_arm(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color, int8_t deltas_fila, int8_t deltas_columna);
extern uint8_t conecta_K_hay_linea_arm_c(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color);
extern uint8_t conecta_K_hay_linea_arm_arm(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color);

// devuelve la longitud de la línea más larga en un determinado sentido
uint8_t conecta_K_buscar_alineamiento_c(TABLERO *t, uint8_t fila,
	uint8_t columna, uint8_t color, int8_t delta_fila, int8_t
	delta_columna)
{
		// comprobar si la celda es valida y del mismo color
		if (tablero_buscar_color(t, fila, columna, color) != EXITO) {
			return 0;
		}
		
    // encontrada, entonces avanzar índices
    uint8_t nueva_fila = fila + delta_fila;
    uint8_t nueva_columna = columna + delta_columna;

    // incrementar longitud y visitar celda vecina
    return 1 + conecta_K_buscar_alineamiento_c(t, nueva_fila, nueva_columna, color, delta_fila, delta_columna);
}

// devuelve true si encuentra una línea de longitud mayor o igual a _K
uint8_t
conecta_K_hay_linea_c_c(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color)
{
	 enum { N_DELTAS = 4};
   int8_t deltas_fila[N_DELTAS] = {0, -1, -1, 1};
   int8_t deltas_columna[N_DELTAS] = {-1, 0, -1, -1};
   unsigned int i = 0;
   uint8_t linea = FALSE;
   uint8_t long_linea = 0;

   // buscar linea en fila, columna y 2 diagonales
   for(i=0; (i < N_DELTAS) && (linea == FALSE); ++i) {
       // buscar sentido
       long_linea = conecta_K_buscar_alineamiento_c(t, fila, columna, color, deltas_fila[i], deltas_columna[i]);
       linea = long_linea >= K_SIZE;
       if (linea) {
         continue;
       }
       // buscar sentido inverso
       long_linea += conecta_K_buscar_alineamiento_c(t, fila-deltas_fila[i],
	       columna-deltas_columna[i], color, -deltas_fila[i], -deltas_columna[i]);


       linea = long_linea >= K_SIZE;
   }
   return linea;
}

// devuelve true si encuentra una línea de longitud mayor o igual a _K
uint8_t
conecta_K_hay_linea_c_arm(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color)
{
	 enum { N_DELTAS = 4};
   int8_t deltas_fila[N_DELTAS] = {0, -1, -1, 1};
   int8_t deltas_columna[N_DELTAS] = {-1, 0, -1, -1};
   unsigned int i = 0;
   uint8_t linea = FALSE;
   uint8_t long_linea = 0;

   // buscar linea en fila, columna y 2 diagonales
   for(i=0; (i < N_DELTAS) && (linea == FALSE); ++i) {
       // buscar sentido
      long_linea = conecta_K_buscar_alineamiento_arm(t, fila, columna, color, deltas_fila[i], deltas_columna[i]);
       linea = long_linea >= K_SIZE;
				if (linea) {
         continue;
       }
       // buscar sentido inverso
			 /*	long_linea += conecta_K_buscar_alineamiento_arm(t, fila-deltas_fila[i],
	       columna-deltas_columna[i], color, -deltas_fila[i], -deltas_columna[i]);
			 */
       long_linea += conecta_K_buscar_alineamiento_arm(t, fila-deltas_fila[i],
	       columna-deltas_columna[i], color, -deltas_fila[i], -deltas_columna[i]);


       linea = long_linea >= K_SIZE;
   }
   return linea;
}

// carga el estado a mitad de partida de las primeras 7 filas y columnas 
// a la estructura de datos tablero para facilitar el test de posibles jugadas
//
// 0: casilla vacia, 1:ficha jugador uno, 2: ficha jugador dos
void conecta_K_test_cargar_tablero(TABLERO *t)
{
	#include "tablero_test.h"	
	
	for (uint8_t i = 0; i < NUM_FILAS; ++i) {
		for (uint8_t j = 0; j < NUM_COLUMNAS; ++j) {
			tablero_insertar_color(t, i, j, tablero_test[i][j]);
		}
	}

	//for... for... tablero_insertar_color...
}

// funcion que visualiza en "pantalla" el contenido de las 7 primeras filas y columnas 
// de las m*n del tablero en juego (suponemos que m y n son >= 7 siempre)
// en memoria se deberia ver algo tal que asi:
// 00 C1 C2 C3 C4 C5 C6 C7
// F1 00 00 00 00 00 00 00
// F2 00 00 11 00 00 00 00
// F3 00 11 22 22 00 00 00
// F4 00 00 11 00 00 00 00
// F5 00 00 00 00 00 00 00
// F6 00 00 00 00 00 00 00
// F7 00 00 00 00 00 00 00 
void conecta_K_visualizar_tablero(TABLERO *t, uint8_t pantalla[8][8])
{
	// Mostrar filas
	for (uint8_t i = 1; i <= NUM_FILAS; i++) {
		pantalla[i][0] = 0xF0 + i;
	}
	
	// Mostrar columnas
	for (uint8_t i = 1; i <= NUM_COLUMNAS; i++) {
		pantalla[0][i] = 0xC0 + i;
	}
	
	// Mostrar el interior del tablero de juego
	CELDA celda;
	uint8_t color;
	for (uint8_t i = 0; i < NUM_FILAS; i++) {
		for (uint8_t j = 0; j < NUM_COLUMNAS; j++) {
			celda = tablero_leer_celda(t, i, j);
			color = celda_color(celda);
			pantalla[i+1][j+1] = (color << 4) + color;
		}
	}
	//.... tablero_leer_celda...
	//... celda_color ....
	
}  


//
uint8_t conecta_K_verificar_K_en_linea(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color){
	// en esta funcion es donde se debe verificar que todas las optimizaciones dan el mismo resultado
	uint8_t resultado_c_c = conecta_K_hay_linea_c_c(t, fila, columna, color);
	uint8_t resultado_c_arm = conecta_K_hay_linea_c_arm(t, fila, columna, color);
	uint8_t resultado_arm_c = conecta_K_hay_linea_arm_c(t, fila, columna, color);
	uint8_t resultado_arm_arm = conecta_K_hay_linea_arm_arm(t, fila, columna, color);
	
	if(!(resultado_c_c == resultado_c_arm && resultado_c_c == resultado_arm_c && resultado_c_c == resultado_arm_arm)){
		while(1); //depura
		
	} 
	return resultado_c_c;
}

/*const int testbench[9][4] = {
	{1, 2, 3, 1},
  {1, 2, 2, 2},
	{1, 7, 7, 2},
	{1, 2, 4, 1},
	{1, 3, 3, 2},
	{1, 1, 1, 1},
	{1, 4, 4, 2},
	{1, 2, 5, 1},
	{1, 5, 5, 2}
};*/

const int testbench[9][4] = {
	{1, 1, 1, 1},
  {1, 1, 2, 2},
	{1, 1, 3, 1},
	{1, 1, 4, 2},
	{1, 1, 5, 1},
	{1, 1, 6, 2},
	{1, 1, 7, 1},
	{1, 2, 5, 2},
	{1, 5, 5, 1}
};

/*const int testbench[3][4] = {
	{1, 4, 4, 2},
	{1, 7, 7, 1},
	{1, 8, 8, 1}
};*/

uint8_t test_index = 0;

void test_entrada(volatile uint8_t entrada[ALIGN_8]) {
		for (uint8_t i = 0; i < 4; i++) {
			entrada[i] = testbench[test_index][i];
		}
		test_index++;
}

void conecta_K_jugar(void){
	// new, row, column, colour, padding to prevent desalinating to 8 bytes
	static volatile uint8_t entrada[8] = {0, 0, 0, 0, 0, 0, 0, 0 }; //jugada, fila, columna, color, ...
	// 8x8 intentando que este alineada para que se vea bien en memoria
	static uint8_t salida[8][8];
	
	TABLERO cuadricula;

	uint8_t row, column, colour;

	tablero_inicializar(&cuadricula);

	//conecta_K_test_cargar_tablero(&cuadricula);
	conecta_K_visualizar_tablero(&cuadricula, salida);

	entrada_inicializar(entrada);
	
	
	while (1){
		test_entrada(entrada); // Banco de pruebas (añade un vector del testbench en entrada)
		entrada_leer(entrada, &row, &column, &colour);
		//validada la entrada en rango, mirar color valido?
		if(tablero_fila_valida(row) && tablero_columna_valida(column) && tablero_color_valido(colour)){	
			//podriamos no validarla ya que tablero_insertar_valor vuelve a validar
			if (celda_vacia(tablero_leer_celda(&cuadricula, row, column))){
				//tablero_insertar tambien chequea si esta libre esa celda o no...
				if(tablero_insertar_color(&cuadricula, row, column, colour) == EXITO) {
					conecta_K_visualizar_tablero(&cuadricula, salida);
					if(conecta_K_verificar_K_en_linea(&cuadricula, row, column, colour)) {
						while(1); // equivaldria a K_linea encontrada, fin de partida... 
					}
				}
				else {
					while(1); //no cabe en la matriz dispersa, hemos dimensionado mal, error de diseño
				}
			}
			//else: celda no vacia
		}
		//else: fuera de rango fila, columna o color
		entrada_inicializar(entrada);
	}
}

// Función original
/*void conecta_K_jugar(void){
	// new, row, column, colour, padding to prevent desalinating to 8 bytes
	static volatile uint8_t entrada[8] = {0, 0, 0, 0, 0, 0, 0, 0 }; //jugada, fila, columna, color, ...
	// 8x8 intentando que este alineada para que se vea bien en memoria
	static uint8_t salida[8][8];
	
	TABLERO cuadricula;

	uint8_t row, column, colour;

	tablero_inicializar(&cuadricula);

	conecta_K_test_cargar_tablero(&cuadricula);
	conecta_K_visualizar_tablero(&cuadricula, salida);

	entrada_inicializar(entrada);
	
	
	while (1){
		while (entrada_nueva(entrada) == 0){};
		entrada_leer(entrada, &row, &column, &colour);
		//validada la entrada en rango, mirar color valido?
		if(tablero_fila_valida(row) && tablero_columna_valida(column) && tablero_color_valido(colour)){	
			//podriamos no validarla ya que tablero_insertar_valor vuelve a validar
			if (celda_vacia(tablero_leer_celda(&cuadricula, row, column))){
				//tablero_insertar tambien chequea si esta libre esa celda o no...
				if(tablero_insertar_color(&cuadricula, row, column, colour) == EXITO) {
					conecta_K_visualizar_tablero(&cuadricula, salida);
					if(conecta_K_verificar_K_en_linea(&cuadricula, row, column, colour)) {
						while(1); // equivaldria a K_linea encontrada, fin de partida... 
					}
				}
				else {
					while(1); //no cabe en la matriz dispersa, hemos dimensionado mal, error de diseño
				}
			}
			//else: celda no vacia
		}
		//else: fuera de rango fila, columna o color
		entrada_inicializar(entrada);
	}
}*/

