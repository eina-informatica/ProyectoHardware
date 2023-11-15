#include "juego.h"

void juego_inicializar(){
    cuenta=0;
    intervalo=0;
    ant_intervalo=0;
    row = 0, column = 0, color = 1;
    final = 0, ganador = 0;
    tiempo_ini = 0, tiempo_fin = 0;
}

void juego_tratar_evento(EVENTO_T ID_evento, uint32_t auxData){
    if (ID_evento==Eint1)
    {
        cuenta++;
        FIFO_encolar(ev_VISUALIZAR_CUENTA,cuenta);
        intervalo=temporizador_drv_leer()-ant_intervalo;
				ant_intervalo = temporizador_drv_leer(); 
        
    }else if (ID_evento==Eint2)
    {
        cuenta--;
        FIFO_encolar(ev_VISUALIZAR_CUENTA,cuenta);
        intervalo=temporizador_drv_leer()-ant_intervalo;
				ant_intervalo = temporizador_drv_leer(); 
    }
}

int juego_leer_cuenta(void){
    return cuenta;
}

int juego_leer_intervalo(void){
    return intervalo;
}


uint8_t conecta_K_hay_linea(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color){
     enum { N_DELTAS = 4};
   int8_t deltas_fila[N_DELTAS] = {0, -1, -1, 1};
   int8_t deltas_columna[N_DELTAS] = {-1, 0, -1, -1};
   unsigned int i = 0;
   uint8_t linea = FALSE;
   uint8_t long_linea = 0;

   // buscar linea en fila, columna y 2 diagonales
   for(i=0; (i < N_DELTAS) && (linea == FALSE); ++i) {
       // buscar sentido
       long_linea = conecta_K_buscar_alineamiento(t, fila, columna, color, deltas_fila[i], deltas_columna[i]);
       linea = long_linea >= K_SIZE;
       if (linea) {
         continue;
       }
       // buscar sentido inverso
       long_linea += conecta_K_buscar_alineamiento(t, fila-deltas_fila[i],
	       columna-deltas_columna[i], color, -deltas_fila[i], -deltas_columna[i]);


       linea = long_linea >= K_SIZE;
   }
   return linea;

}

uint8_t conecta_K_buscar_alineamiento(TABLERO *t, uint8_t fila,
	uint8_t columna, uint8_t color, int8_t delta_fila, int8_t
	delta_columna)
{
	uint8_t nueva_fila;
	 uint8_t nueva_columna ;
		// comprobar si la celda es valida y del mismo color
		if (tablero_buscar_color(t, fila, columna, color) != EXITO) {
			return 0;
		}
		
    // encontrada, entonces avanzar índices
    nueva_fila = fila + delta_fila;
    nueva_columna = columna + delta_columna;

    // incrementar longitud y visitar celda vecina
    return 1 + conecta_K_buscar_alineamiento(t, nueva_fila, nueva_columna, color, delta_fila, delta_columna);
}


void conecta_K_visualizar_tablero(CELDA cuadricula[7][8]){
			unsigned int i,j;
		   unsigned int indice;
	
    // Se inicializa el buffer 'tablero' con ceros
    memset(tablero, 0, 200);

    // Variable para el índice en el buffer 'tablero'
			indice= 0;

    // Ciclo para recorrer las filas del tablero en orden descendente
    for (i = 6; i > 1; i--) {
        // Se añade el número de la fila al buffer 'tablero'
        tablero[indice] = i + '0';
        tablero[indice] = '|';

        // Ciclo para recorrer las columnas del tablero
        for (j = 1; j < 7; j++) {
            // Se determina el contenido de la celda y se añade al buffer 'tablero'
            if (cuadricula[i][j] == 0x05) {
                tablero[indice] = 'B';
            } else if (cuadricula[i][j] == 0x06) {
                tablero[indice] = 'N';
            } else if (cuadricula[i][j] == 0x00) {
                tablero[indice] = ' ';
            }
            tablero[indice] = '|';
        }
        // Se agrega un salto de línea al buffer 'tablero'
        tablero[indice] = '\n';
    }

    // Ciclo para agregar una línea divisoria en la parte inferior del tablero
    for ( i = 1; i < 16; i++) {
        tablero[indice] = '-';
    }

    // Se añade un salto de línea al buffer 'tablero'
    tablero[indice] = '\n';

    // Se añade un '-' y '|' para etiquetar las columnas del tablero
    tablero[indice] = '-';
    tablero[indice] = '|';

    // Ciclo para añadir los números de las columnas al buffer 'tablero'
    for ( i = 1; i < 7; i++) {
        tablero[indice] = i + '0';
        tablero[indice] = '|';
    }

    // Se añade un salto de línea al buffer 'tablero'
    tablero[indice] = '\n';

    // Se añade un salto de línea adicional al buffer 'tablero'
    tablero[indice] = '\n';

    // Se envía el contenido del buffer 'tablero' a través de UART0
    linea_serie_drv_enviar_array(tablero);
}
// Función para reiniciar un tablero de juego
void conecta_K__reiniciar_tablero(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]) {
		unsigned int i,j;
    // Ciclo para recorrer las filas del tablero
    for ( i = 1; i < NUM_FILAS; i++) {
        // Ciclo para recorrer las columnas del tablero
        for ( j = 1; j < NUM_COLUMNAS; j++) {
            // Se llama a la función 'celda_poner_valor' para establecer el valor de la celda en 0
            celda_poner_valor(&cuadricula[i][j], 0);
        }
    }
}

