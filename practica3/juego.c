#include "juego.h"

enum Estado {
    INICIO,
		INICIO_ESPERA,
    TURNO_NUEVO,
		TURNO_NUEVO_ESPERA,
    JUGADA_NUEVA,
		JUGADA_NUEVA_ESPERA,
    FINAL,
		FINAL_ESPERA
};

static void (*callback_f)(EVENTO_T, uint32_t);
static void (*callback_f2)(char*);
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
static TABLERO cuadricula;
static uint32_t t_init,t_fini,t_total;
static char comandito[4];
//static int hecho=0;
static enum Estado estado;
static int linea_serie_libre = 1; // ¿Línea serie está libre?
static int contador_turno;

// Función invocada tras tratar un evento para gestionar un potencial cambio de estado
void gestionar_estado() {
    switch(estado) {
        case INICIO:
            // Mostrar instrucciones
						linea_serie_libre = 0;
            callback_f2("Has iniciado el juego ConectaK.\nUn juego de dos jugadores cuyo objetivo es completar una linea de K elementos.\nEn cada turno, el jugador puede introducir por consola $F-C! donde F es la fila y C la columna \nen la que quiere colocar ficha. A continuacion se mostrara por pantalla una previsualizacion\nde como quedaria la jugada, tras 3 segundos se confirmara la jugada sino se cancela pulsando el boton.\nEn cualquier momento se puede cancelar la partida pulsando el boton 2 o introduciento $END! por consola.\n");
            estado = INICIO_ESPERA;
            break;
        case TURNO_NUEVO:
            // Mostrar tablero
            conecta_K_visualizar_tablero();
            
            //conecta_K_visualizar_tablero();
            break;
        case JUGADA_NUEVA:
            // Mostrar tablero con futura ficha colocada
            callback_f2("La jugada se confirmará en 3 segundos\n Pulsa el botón 1 para cancelar.\n");
        case FINAL:
            // Mostrar ganador
            callback_f2("PARTIDA ACABADA\n");
        default:
            break;
    }
}

void juego_inicializar(void (*callback)(EVENTO_T, uint32_t), void (*callback2)(char*)){
    callback_f = callback; // Encolar en FIFO
    callback_f2 = callback2; // Enviar por línea serie
    cuenta=0;
    intervalo=0;
    ant_intervalo=0;
    row = 0, column = 0, color = 1;
    final = 0, ganador = 0;
    tiempo_ini = 0, tiempo_fin = 0;
    t_fini=0, t_init=0,t_total=0;
    estado = INICIO;
    contador_turno = 0;
		
	tablero_inicializar(&cuadricula);
	gestionar_estado(); // Nada más inicializar mostramos instrucciones por pantalla
}

void juego_tratar_evento(EVENTO_T ID_evento, uint32_t auxData){
    switch(ID_evento) {
        case Eint1: 
            cuenta++;
            callback_f(ev_VISUALIZAR_CUENTA,cuenta);
            intervalo=temporizador_drv_leer()-ant_intervalo;
            ant_intervalo = temporizador_drv_leer();
            if (estado == JUGADA_NUEVA) {
                estado = TURNO_NUEVO;
            }
            break;
        case Eint2:
            cuenta--;
            callback_f(ev_VISUALIZAR_CUENTA,cuenta);
            intervalo=temporizador_drv_leer()-ant_intervalo;
            ant_intervalo = temporizador_drv_leer(); 
            break;
            estado = FINAL;
        case ev_RX_SERIE:
            // aux=(char*) &auxData;
            // comandito[1]=aux[1];
            comandito[0] = (auxData >> 16) & 0xFF;  // Byte más significativo
            comandito[1] = (auxData >> 8) & 0xFF;
            comandito[2] = auxData & 0xFF;  // Byte menos significativo
            comandito[3] = '\0';  // Añade el carácter nulo para terminar la cadena
            if (strcmp(comandito,"END")==0){
                tablero_inicializar(&cuadricula);
                //callback_f2("PARTIDA ACABADA\n");
                estado = FINAL;
            }else if (strcmp(comandito,"NEW")==0){
								if (linea_serie_libre && estado == INICIO_ESPERA) {
                                        linea_serie_libre = 0;
										estado = TURNO_NUEVO;
                                        contador_turno++;
										gestionar_estado();
								}
                // juego_inicializar();
                //callback_f2("PARTIDA EMPEZADA\n");
								/*if (estado == INICIO || estado == FINAL) {
										estado = INICIO_ESPERA;
										gestionar_estado();
								}*/
            }else if (strcmp(comandito,"TAB")==0){
                t_init=clock_getus();
                conecta_K_visualizar_tablero();
            }else if (comandito[1]=='-'&& isdigit(comandito[2]) && isdigit(comandito[0])){
                //hacer _jugada(aux[0],aux[2])
            }
            break;
        case ev_TX_SERIE:
            if((strcmp(comandito,"TAB")==0)/* && (hecho==0)*/){
                t_fini=clock_getus();
                t_total=t_fini-t_init;
                conecta_K_visualizar_tiempo(t_total);
                //hecho=1;
						/*}else if(hecho==1){
								hecho=0;
								callback_f(restart,0);*/
						} else {
								linea_serie_libre = 1;
						}
            break;
        default:
            break;
    }
}

// tenemoS que convertir el uint que recibimos en un char* para poder enviarlo por la linea serie
void conecta_K_visualizar_tiempo(uint32_t tiempo){
        char tiempo_char[33];
        uint32_to_char(tiempo, tiempo_char);
        callback_f2(tiempo_char);
}

void uint32_to_char(uint32_t num, char* str) {
    int i = 0;
    uint32_t temp = num;
    uint32_t divisor = 1;

    // Calcular el divisor necesario para obtener el dígito más significativo
    while (temp / divisor >= 10) {
        divisor *= 10;
    }

    // Convertir cada dígito en un carácter y lo almacena en la cadena
    while (divisor > 0) {
        uint32_t digit = num / divisor;
        str[i++] = digit + '0';
        num %= divisor;
        divisor /= 10;
    }
		
		// Agregar unidad de tiempo
		str[i++] = ' ';
		str[i++] = 'u';
		str[i++] = 's';

    // Agregar el carácter nulo al final de la cadena
    str[i++] = '\n';
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

void conecta_K_visualizar_tablero(void) {
    const char cabecera1[] = "Turno del 1er moro\n";
    const char cabecera2[] = "Turno del 2do moro\n";
    static char tablero[200];
    unsigned int indice = 0;
    unsigned int i, j;
     if (contador_turno%2==1){
        color=0;
        strcpy(tablero, cabecera1);
        indice = strlen(cabecera1);
    } else {
        color=1;
        strcpy(tablero, cabecera2);
        indice = strlen(cabecera2);
    }
    // Ciclo para recorrer las filas del tablero en orden descendente
    for (i = MAX_NO_CERO; i > 0; i--) {
        // Se añade el número de la fila al buffer 'tablero'
        tablero[indice++] = i + '0';
        tablero[indice++] = '|';

        // Ciclo para recorrer las columnas del tablero
        for (j = 1; j <= NUM_FILAS; j++) {
            CELDA celda = tablero_leer_celda(&cuadricula, i, j);
            // Se determina el contenido de la celda y se añade al buffer 'tablero'
            if (celda == 0x00) {
                // Celda vacía
                tablero[indice++] = ' ';
            } else if (celda == 0x05) {
                // Celda ocupada por jugador 1
                tablero[indice++] = 'B';
            } else if (celda == 0x06) {
                // Celda ocupada por jugador 2
                tablero[indice++] = 'N';
            }
            tablero[indice++] = '|';
        }

        tablero[indice++] = '\n';
    }
    tablero[indice++] = '-';
    tablero[indice++] = '|';

    // Ciclo para añadir los números de las columnas al buffer 'tablero'
    for ( i = 1; i <= NUM_FILAS; i++) {
        tablero[indice++] = i + '0';
        tablero[indice++] = '|';
    }

    tablero[indice++] = '\n';
    tablero[indice] = '\0';

    callback_f2(tablero);
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
        } else {
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
char* reverse(char *buffer, int i, int j){
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
		
    return buffer;
}

