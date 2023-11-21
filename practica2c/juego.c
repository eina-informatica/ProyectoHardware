#include "juego.h"

void juego_inicializar(void){
    cuenta=0;
    intervalo=0;
    ant_intervalo=0;
    row = 0, column = 0, color = 1;
    final = 0, ganador = 0;
    tiempo_ini = 0, tiempo_fin = 0;
    t_fini=0, t_init=0,t_total=0;
		
		tablero_inicializar(&cuadricula);
}

void juego_tratar_evento(EVENTO_T ID_evento, uint32_t auxData){
        char comandito[4];
		switch(ID_evento) {
				case Eint1: 
						cuenta++;
  					FIFO_encolar(ev_VISUALIZAR_CUENTA,cuenta);
						intervalo=temporizador_drv_leer()-ant_intervalo;
						ant_intervalo = temporizador_drv_leer(); 
						break;
				case Eint2:
						cuenta--;
						FIFO_encolar(ev_VISUALIZAR_CUENTA,cuenta);
						intervalo=temporizador_drv_leer()-ant_intervalo;
						ant_intervalo = temporizador_drv_leer(); 
						break;
				case ev_RX_SERIE:
							// aux=(char*) &auxData;
							// comandito[1]=aux[1];
                            comandito[0] = (auxData >> 16) & 0xFF;  // Byte más significativo
							comandito[1] = (auxData >> 8) & 0xFF;
							comandito[2] = auxData & 0xFF;  // Byte menos significativo
							comandito[3] = '\0';  // Añade el carácter nulo para terminar la cadena
							if (strcmp(comandito,"END")==0){
									tablero_inicializar(&cuadricula);
								linea_serie_drv_enviar_array("PARTIDA ACABADA :  ");
							}else if (strcmp(comandito,"NEW")==0){
									juego_inicializar();
								linea_serie_drv_enviar_array("PARTIDA EMPEZADA : ");
							}else if (strcmp(comandito,"TAB")==0){
                  t_init=clock_getus();
									conecta_K_visualizar_tablero();
							}else if (comandito[1]=='-'&& isdigit(comandito[2]) && isdigit(comandito[0])){

									//hacer _jugada(aux[0],aux[2])
						}
						break;
                case ev_TX_SERIE:
										t_fini=clock_getus();
								t_total=t_fini-t_init;
                    conecta_K_visualizar_tiempo(t_total);
                        break;
				default:
						break;
		}
}
//tenemo que convertir el uint que recibimos en un char* para poder enviarlo por la linea serie
void conecta_K_visualizar_tiempo(uint32_t tiempo){
        char tiempo_char[33];
        uint32_to_char(tiempo, tiempo_char);
        linea_serie_drv_enviar_array(tiempo_char);
  
}

void uint32_to_char(uint32_t num, char* str) {
    int i = 0;
    uint32_t temp = num;
    uint32_t divisor = 1;

    // Calcula el divisor necesario para obtener el dígito más significativo
    while (temp / divisor >= 10) {
        divisor *= 10;
    }

    // Convierte cada dígito en un carácter y lo almacena en la cadena
    while (divisor > 0) {
        uint32_t digit = num / divisor;
        str[i++] = digit + '0';
        num %= divisor;
        divisor /= 10;
    }

    // Agrega el carácter nulo al final de la cadena
    str[i] = '\n';
    i++;
    str[i] = '\0';
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

void conecta_K_visualizar_tablero(void){
	  unsigned int i,j;
		unsigned int indice;
		char * aux;
		char * tablero;
		tablero=aux;
	
	

    // Variable para el índice en el buffer 'tablero'
		indice = 0;
	

    // Ciclo para recorrer las filas del tablero en orden descendente
    for (i = MAX_NO_CERO; i > 0; i--) {
        // Se añade el número de la fila al buffer 'tablero'
        aux[indice++] = i + '0';
        aux[indice++] = '|';

        // Ciclo para recorrer las columnas del tablero
        for (j = 1; j <= NUM_FILAS; j++) {
			CELDA celda = tablero_leer_celda(&cuadricula, i, j);
            // Se determina el contenido de la celda y se añade al buffer 'tablero'
            if (celda == 0x05) {
                aux[indice++] = 'B';
            } else if (celda == 0x06) {
                aux[indice++] = 'N';
            } else if (celda == 0x00) {
                aux[indice++] = ' ';
            }
            aux[indice++] = '|';
        }
        // Se agrega un salto de línea al buffer 'tablero'
        aux[indice++] = '\n';
    }

    // Ciclo para agregar una línea divisoria en la parte inferior del tablero
    /*for ( i = 1; i < 16; i++) {
        tablero[indice++] = '-';
    }*/

    // Se añade un salto de línea al buffer 'tablero'
    //tablero[indice++] = '\n';

    // Se añade un '-' y '|' para etiquetar las columnas del tablero
    aux[indice++] = '-';
    aux[indice++] = '|';

    // Ciclo para añadir los números de las columnas al buffer 'tablero'
    for ( i = 1; i <= NUM_FILAS; i++) {
        aux[indice++] = i + '0';
        aux[indice++] = '|';
    }

    // Se añade un salto de línea al buffer 'tablero'
    aux[indice++] = '\n';

    // Se añade un salto de línea adicional al buffer 'tablero'
    aux[indice++] = '\n';

    // Se envía el contenido del buffer 'tablero' a través de UART0
    linea_serie_drv_enviar_array(tablero);
		//linea_serie_drv_enviar_array("eina");
}

char* itoa(int value, char* buffer, int base)
{
		int i, n,r;
    // entrada inválida
    if (base < 2 || base > 32) {
        return buffer;
    }
 
    // considera el valor absoluto del número
    n = abs(value);
 
    i = 0;
    while (n)
    {
        r = n % base;
 
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }
 
        n = n / base;
    }
 
    // si el numero es 0
    if (i == 0) {
        buffer[i++] = '0';
    }
 
    // Si la base es 10 y el valor es negativo, la string resultante
    // va precedido de un signo menos (-)
    // Con cualquier otra base, el valor siempre se considera sin firmar
    if (value < 0 && base == 10) {
        buffer[i++] = '-';
    }
    buffer[i] = '\0'; // string de terminación nula
		
    // invertir la string y devolverla
    return reverse(buffer, 0, i - 1);;
}
void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}
 
// Función para invertir `buffer[i…j]`
char* reverse(char *buffer, int i, int j)
{
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
		
    return buffer;
}

