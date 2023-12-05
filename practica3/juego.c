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
static void (*callback_alarma)(EVENTO_T, uint32_t, uint32_t);
static uint32_t (*callback_stats)(EVENTO_T);
static uint32_t cuenta;
static uint32_t intervalo;
static uint32_t ant_intervalo;
static char tablero[400];
//static CELDA celda;
static uint8_t fila = 0;
static uint8_t columna = 0;
static uint8_t color = 1;
/*static unsigned int final = 0;
static unsigned int ganador = 0;
*/
static TABLERO cuadricula;
static uint32_t t_init,t_fini,t_total;
static char comandito[4];
static int hecho=0;
static enum Estado estado;
static int linea_serie_libre = 1; // ¿Línea serie está libre?
static int primer_turno;
static int turno;
static int jugador;
static int jugador_ant;
static int cancelada=0;
static int acabo=0;
static int rendir=0;
static int mal=0;
static int travieso=0;
static uint32_t tiempo_ini_proces;
static uint32_t tiempo_fin_proces;
static uint32_t tiempo_ini_hay_linea;
static uint32_t tiempo_fin_hay_linea;
static uint32_t tiempo_ini_pensar;
static uint32_t tiempo_fin_pensar;
static uint32_t tiempo_total_pensar;
static uint32_t tiempo_total_hay_linea;
static int calculos=0;
static int calc=0;
static int hay=0;
static int numVecesAnt[max_Eventos-2] = {0};



int isDigit(char c) {
    return (c >= '0' && c <= '9');
}
int jug_global(void){
    if ((jugador_ant%2)==0){
        if (rendir){
            return 1;
        }
        return 2;
    }else{
        if (rendir){
            return 2;
        }
        return 1;
    }
}

// Función invocada tras tratar un evento para gestionar un potencial cambio de estado
void gestionar_estado() {
		char ply[20];
    char cabecera6[] = "El ganador es el jugador ";
    switch(estado) {
        case INICIO:
            // Mostrar instrucciones
			linea_serie_libre = 0;
            callback_f2("Has iniciado el juego ConectaK.\nUn juego de dos jugadores cuyo objetivo es completar una linea de K elementos.\nEn cada turno, el jugador puede introducir por consola $F-C! donde F es la fila y C la columna \nen la que quiere colocar ficha. A continuacion se mostrara por pantalla una previsualizacion\nde como quedaria la jugada, tras 3 segundos se confirmara la jugada sino se cancela pulsando el boton.\nEn cualquier momento se puede cancelar la partida pulsando el boton 2 o introduciento $END! por consola.\n");
            estado = INICIO_ESPERA;
            break;
        case TURNO_NUEVO:
            // Mostrar tablero
            if (primer_turno > 0) {
                color=jug_global();
                //introducir_ficha(&cuadricula,jugador);
                conecta_K_visualizar_tablero(jug_global());
            }else if (turno%2==0)
            {
                jugador++;
				color=(jug_global()+jugador+1)%2+1;
                conecta_K_visualizar_tablero((jug_global()+jugador+1)%2+1);
            }else if (turno%2==1){
								color=(jug_global()+jugador+1)%2+1;
                conecta_K_visualizar_tablero((jug_global()+jugador+1)%2+1);
            }
            tiempo_ini_pensar=clock_getus();
            //conecta_K_visualizar_tablero();
            
            //conecta_K_visualizar_tablero();
            break;
        case JUGADA_NUEVA:
            
            // Mostrar tablero con futura ficha colocada
        case FINAL:
            // Mostrar ganador
            if (primer_turno > 0) {
							//primer_turno--;
             uint32_to_char(jug_global(),ply);
             strcat(cabecera6,ply);
            callback_f2(cabecera6);
                
            }else if (turno%2==0)
            {
                uint32_to_char((jug_global()+jugador+1)%2+1,ply);
                strcat(cabecera6,ply);
                callback_f2(cabecera6);
            }else if (turno%2==1){
                uint32_to_char((jug_global()+jugador+1)%2+1,ply);
                strcat(cabecera6,ply);
                callback_f2(cabecera6);
            }
						break;
        case FINAL_ESPERA:
            // Mostrar tablero
            conecta_K_visualizar_tablero(15);
            break;
	
           
        default:
            break;
    }
}
// carga el estado a mitad de partida de las primeras 7 filas y columnas 
// a la estructura de datos tablero para facilitar el test de posibles jugadas
//
// 0: casilla vacia, 1:ficha jugador uno, 2: ficha jugador dos
void conecta_K_test_cargar_tablero(TABLERO *t)
{
	#include "tablero_test.h"	
    uint8_t i,j;
	
	for (i = 0; i < NUM_FILAS; ++i) {
		for (j = 0; j < NUM_COLUMNAS; ++j) {
			tablero_insertar_color(t, i, j, tablero_test[i][j]);
		}
	}
}

char* eventoToString(EVENTO_T evento) {
    switch (evento) {
        case Timer0:
            return "Timer0";
            
        case Timer1:
            return "Timer1";
            
        case ALARMA_OVERFLOW:
            return "ALARMA_OVERFLOW";            
        case Eint1:
            return "Eint1";
        case Eint2:
					return "Eint2";
        case comprobar1:
            return "comprobar1";
            
        case comprobar2:
            return "comprobar2";
            
        case dormir:
            return "dormir";
            
        case ev_VISUALIZAR_CUENTA:
            return "ev_VISUALIZAR_CUENTA";
            
        case ev_LATIDO:
            return "ev_LATIDO";
            
        case ev_VISUALIZAR_HELLO:
            return "ev_VISUALIZAR_HELLO";
            
        case ev_RX_SERIE:
            return "ev_RX_SERIE";
            
        case ev_TX_SERIE:
            return "ev_TX_SERIE";
            
        case restart:
            return "restart";
            
        case ev_confirmacion:
            return "ev_confirmacion";
            
        case kk:
            return "kk";
            
        default:
            return"";
    }
}


void juego_inicializar(void (*callback)(EVENTO_T, uint32_t), void (*callback2)(char*),
                            void (*callback3)(EVENTO_T, uint32_t, uint32_t),uint32_t callback4(EVENTO_T)){
    callback_f = callback; // Encolar en FIFO
    callback_f2 = callback2; // Enviar por línea serie
    callback_alarma = callback3;
    callback_stats=callback4;
    cuenta=0;
    intervalo=0;
    ant_intervalo=0;
    jugador_ant=1;
    tiempo_ini_proces=0;
    tiempo_fin_proces=0;
    tiempo_ini_hay_linea=0;
    tiempo_fin_hay_linea=0;
    tiempo_ini_pensar=0;
    tiempo_fin_pensar=0;
    primer_turno=Q_SIZE;
    turno=0;
  /*  row = 0, column = 0,*/ color = 1;
//    final = 0, ganador = 0;
    t_fini=0, t_init=0,t_total=0;
    estado = INICIO;
		
	tablero_inicializar(&cuadricula);
    conecta_K_test_cargar_tablero(&cuadricula);
	gestionar_estado(); // Nada más inicializar mostramos instrucciones por pantalla
}

void juego_tratar_evento(EVENTO_T ID_evento, uint32_t auxData){
		int i;
//        int j;
    char aux_fin[75];
	char aux_ini[75];
    char cabecera_tiempo_total[] = "Tiempo total de uso del procesador: \n";
    char cabecera_tiempo_hay_linea[] = "Tiempo de uso del procesador en la funcion hay_linea: \n";
    char cabecera_tiempo_pensar[] = "Tiempo pensando: \n";
    char cabecera_events[400]= "Numero de eventos encolados: \n";
    
    switch(ID_evento) {
        case Eint1: 
            cuenta++;
            callback_f(ev_VISUALIZAR_CUENTA,cuenta);
            intervalo=temporizador_drv_leer()-ant_intervalo;
            ant_intervalo = temporizador_drv_leer();
            if (estado == JUGADA_NUEVA_ESPERA)
            {
                callback_alarma(ev_confirmacion, 0, 0);
                callback_f2("Movimiento cancelado\n");
								cancelada=1;
                
            }
            if (linea_serie_libre && estado == INICIO_ESPERA) {
                linea_serie_libre = 0;
                estado = TURNO_NUEVO;
                gestionar_estado();
            }
            break;
        case Eint2:
            cuenta--;
            callback_f(ev_VISUALIZAR_CUENTA,cuenta);
            intervalo=temporizador_drv_leer()-ant_intervalo;
            ant_intervalo = temporizador_drv_leer(); 
            if (estado == TURNO_NUEVO)
            {
                callback_alarma(ev_confirmacion, 0, 0);
                tiempo_fin_pensar=clock_getus();
                tiempo_total_pensar+=tiempo_fin_pensar-tiempo_ini_pensar;
                
                callback_f2("RENDICION\n");
                
                rendir=1;
            }
            if (linea_serie_libre && estado == INICIO_ESPERA) {
                linea_serie_libre = 0;
                estado = TURNO_NUEVO;
                gestionar_estado();
            }
            break;
        case ev_RX_SERIE:
            // aux=(char*) &auxData;
            // comandito[1]=aux[1];
						
            comandito[0] = (auxData >> 16) & 0xFF;  // Byte más significativo
            comandito[1] = (auxData >> 8) & 0xFF;
            comandito[2] = auxData & 0xFF;  // Byte menos significativo
            comandito[3] = '\0';  // Añade el carácter nulo para terminar la cadena
            if (strcmp(comandito,"END")==0){
                //callback_f2("PARTIDA ACABADA\n");
                if (estado == TURNO_NUEVO)
                {
                    callback_alarma(ev_confirmacion, 0, 0);
                    tiempo_fin_pensar=clock_getus();
                    tiempo_total_pensar+=tiempo_fin_pensar-tiempo_ini_pensar;
                        
                    callback_f2("RENDICION\n");
                    rendir=1;
                }
            }else if (strcmp(comandito,"NEW")==0){
								if (linea_serie_libre && estado == INICIO_ESPERA) {
                                        tiempo_ini_proces=clock_getus();
                                        linea_serie_libre = 0;
										estado = TURNO_NUEVO;
										gestionar_estado();
								}else if (FINAL_ESPERA && travieso==1){
                                    tablero_inicializar(&cuadricula);
                                    turno=0;
                                    jugador=0;
                                    acabo=0; 
                                    tiempo_ini_proces=clock_getus();
																		calculos=0;
																		calc=0;
																		tiempo_total_hay_linea=0;
																		tiempo_total_pensar=0;
                                    conecta_K_test_cargar_tablero(&cuadricula);
									
                                    jugador_ant++;
                                    estado = TURNO_NUEVO;
                                    travieso=0;
                                    gestionar_estado();
                                }
                // juego_inicializar();
                //callback_f2("PARTIDA EMPEZADA\n");
								/*if (estado == INICIO || estado == FINAL) {
										estado = INICIO_ESPERA;
										gestionar_estado();
								}*/
            }else if (strcmp(comandito,"TAB")==0){
                hecho=0;
                t_init=clock_getus();
                conecta_K_visualizar_tablero(15);
            }else if (comandito[1]=='-'){
                if (estado == TURNO_NUEVO){
                    if(isDigit(comandito[2]) && isDigit(comandito[0])){
                        if (!tablero_fila_valida((comandito[0]-'0')-1)) {
                            callback_f2("Fila no valida \n");
                        } else if (!tablero_columna_valida((comandito[2]-'0')-1)) {
                            callback_f2("Columna no valida \n");
                        } else {
                            fila=comandito[0]-'0';
                            columna=comandito[2]-'0';
                            tiempo_fin_pensar=clock_getus();
                            tiempo_total_pensar+=tiempo_fin_pensar-tiempo_ini_pensar;
                            estado=JUGADA_NUEVA;
														// Se resta 1, porque el usuario introduce valores en el intervalo [1, {NUM_FILAS, NUM_COLUMNAS}], 
														// sin embargo el programa acepta valores en el intervalo [0, {NUM_FILAS, NUM_COLUMNAS} - 1]
                            visualizar_jugada(comandito[0]-1-'0',comandito[2]-1-'0');
                        }
                   
                    }
                }
            }else{
                gpio_hal_escribir(29,1,1);
                callback_f2("Comando no valido\n");

            }
            break;
        case ev_TX_SERIE:
            if((strcmp(comandito,"TAB")==0) && (hecho==0)){
                t_fini=clock_getus();
                t_total=t_fini-t_init;
                conecta_K_visualizar_tiempo(t_total);
                hecho=1;
						}else if(estado == FINAL){
                            estado = FINAL_ESPERA;
                            gestionar_estado();		
						} else if (estado == JUGADA_NUEVA_ESPERA && cancelada==1) {
                            estado = TURNO_NUEVO;
                            gestionar_estado();
                            cancelada=0;
                        }else if (estado == JUGADA_NUEVA_ESPERA && acabo==1){
                            estado = FINAL;
                            gestionar_estado();
                        }else if (estado == TURNO_NUEVO && rendir==1){
                            estado = FINAL;
                            gestionar_estado();
                            rendir=0;
                        }else if (estado == TURNO_NUEVO && mal==1){
                            mal=0;
                        }else if (estado == FINAL_ESPERA && calculos==0){
                                switch (calc) {
                                    case 0:
                                        tiempo_fin_proces = clock_getus();
                                        uint32_to_char(tiempo_fin_proces - tiempo_ini_proces, aux_fin);
                                        strcat(cabecera_tiempo_total, aux_fin);
                                        calc++;
                                        callback_f2(cabecera_tiempo_total);
                                        break;
                                    case 1:
                                        uint32_to_char(tiempo_total_hay_linea, aux_fin);
                                        strcat(cabecera_tiempo_hay_linea, aux_fin);
                                        calc++;
                                        callback_f2(cabecera_tiempo_hay_linea);
                                        break;
                                    case 2:
                                        uint32_to_char(tiempo_total_pensar, aux_fin);
                                        strcat(cabecera_tiempo_pensar, aux_fin);
                                        calc++;
                                        callback_f2(cabecera_tiempo_pensar);
                                        break;
                                    case 3:
                                        //total de eventos encolados
                                         // Iterar sobre todos los eventos y mostrar las estadísticas
                                         
                                        calculos=1;
                                        for (i = 0; i < max_Eventos-2; ++i) {
                                            EVENTO_T evento = (EVENTO_T)i;
                                            uint32_t numVeces = callback_stats(evento);
                                            numVeces=numVeces-numVecesAnt[i];
                                            uint32_to_char(numVeces, aux_fin);
                                            numVecesAnt[i]+=numVeces;

                                            // j = 0;
                                            // while (eventoToString(evento)[j] != '\0') {
                                            //     aux_ini[j] = eventoToString(evento)[j];
                                            //     j++;
                                            // }
                                            // aux_ini[j] = '\0';
                                            strcpy(aux_ini, eventoToString(evento));																					
                                            strcat(cabecera_events,aux_ini);
                                            strcat(cabecera_events, ": ");
                                            strcat(cabecera_events, aux_fin);
                                            
                                            // Imprimir el resultado en el formato deseado
                                        }
                                      
                                        callback_f2(cabecera_events);
																			
                                        break;
                                    default:
                                        // Add your code here for other cases
                                        break;
                                }

                            }
                        else if (estado==FINAL_ESPERA && travieso==0){

                            callback_f2("Quieres jugar otra partida?\n \tPulsa $NEW! para empezar una nueva partida\n");
                            travieso=1;
                        }
                        else {
								linea_serie_libre = 1;
						}
            break;
        case ev_confirmacion:
            if (estado == JUGADA_NUEVA_ESPERA) {
                tablero_insertar_color(&cuadricula, fila-1, columna-1, color);
                    tiempo_ini_hay_linea=clock_getus();
                    hay=conecta_K_hay_linea(&cuadricula, fila-1, columna-1, color);
                    tiempo_fin_hay_linea=clock_getus();
                    tiempo_total_hay_linea+=tiempo_fin_hay_linea-tiempo_ini_hay_linea;
                if (hay) {
                    acabo=1;
                    callback_f2("PARTIDA ACABADA\n");
                }else if(primer_turno > 0){
                    primer_turno--;
                    estado = TURNO_NUEVO;
                }else{
                    turno++;
                    estado = TURNO_NUEVO;
                }
                gestionar_estado();
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
        if (!(estado==FINAL || calculos==1))
        {
		str[i++] = ' ';
		str[i++] = 'u';
		str[i++] = 's';
        }
        

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


void conecta_K_visualizar_tablero(int player) {
    const char cabecera1[] = "Turno del 1er moro\n";
    const char cabecera2[] = "Turno del 2do moro\n";
    const char cabecera3[] = "Mostrar tablero\n";
    unsigned int indice = 0;
    int i, j;
     if (player==1){
        strcpy(tablero, cabecera1);
        indice = strlen(cabecera1);
    } else if (player==2) {
        strcpy(tablero, cabecera2);
        indice = strlen(cabecera2);
    }else if (player==15){
        strcpy(tablero, cabecera3);
        indice = strlen(cabecera3);
        
    }
    // Ciclo para recorrer las filas del tablero en orden descendente
    for (i = NUM_FILAS - 1; i >= 0; i--) {
        // Se añade el número de la fila al buffer 'tablero'
        tablero[indice++] = i + 1 + '0';
        tablero[indice++] = '|';

        // Ciclo para recorrer las columnas del tablero
        for (j = 0; j < NUM_COLUMNAS; j++) {
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

void visualizar_jugada(uint32_t fila, uint32_t columna){
    unsigned int indice = 0;
    const char cabecera3[] = "Jugada erronea, posicion ya ocupada\n";
		const char cabecera4[] = "Previsualizacion de la jugada\n";
    const char cabecera5[] = "Pulsa boton 1 para cancelar o espera 3 segundos para confirmar\n";

    int i, j;
		strcpy(tablero, cabecera4);
    indice += strlen(cabecera4);
    // Ciclo para recorrer las filas del tablero en orden descendente
    //for (i = MAX_NO_CERO; i > 0; i--) {
		for (i = NUM_FILAS - 1; i >= 0; i--) {
        // Se añade el número de la fila al buffer 'tablero'
        tablero[indice++] = i + 1 + '0';
        tablero[indice++] = '|';

        // Ciclo para recorrer las columnas del tablero
        //for (j = 1; j <= NUM_FILAS; j++) {
				for (j = 0; j < NUM_COLUMNAS; j++) {
            CELDA celda = tablero_leer_celda(&cuadricula, i, j);
            // Se determina el contenido de la celda y se añade al buffer 'tablero'
            if (celda == 0x00) {
                // Celda vacía
                if ((i==fila) && (j==columna)) {
									tablero[indice++] = '#';
                } else {
									tablero[indice++] = ' ';
								}
            } else if (celda == 0x05) {
                // Celda ocupada por jugador 1
                if ((i==fila) && (j==columna)) {
                    mal=1;
                }
                tablero[indice++] = 'B';
            } else if (celda == 0x06) {
                // Celda ocupada por jugador 2
                if ((i==fila) && (j==columna)) {
                    mal=1;
                }
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
    if (mal==1){
        tablero[indice++] = '\n';
        for (i = 0; cabecera3[i] != '\0'; i++) {
            tablero[indice++] = cabecera3[i];
        }
        // strcat(tablero, cabecera3);
        // indice += strlen(cabecera3);
        estado=TURNO_NUEVO;
    }else{
        tablero[indice++] = '\n';
        for (i = 0; cabecera5[i] != '\0'; i++) {
        tablero[indice++] = cabecera5[i];
         }
        // strcat(tablero, cabecera5);
        // indice += strlen(cabecera5);
        estado = JUGADA_NUEVA_ESPERA;
        callback_alarma(ev_confirmacion, 3000, 0);
    }
    tablero[indice++] = '\n';
    tablero[indice] = '\0';

    callback_f2(tablero);
}/*
void introducir_ficha(char tablero[], CELDA* cuadricula, int player) {
    // Obtener la posición de la ficha en el tablero
    int i;
    int j;
    int fila = 0;
    int columna = 0;
    for (i = 0; i < NUM_FILAS; i++) {
        for (j = 0; j < NUM_COLUMNAS; j++) {
            if (tablero[i * (NUM_COLUMNAS + 1) + j] == '#') {
                fila = i + 1;
                columna = j + 1;
                break;
            }
        }
    }

    // Insertar la ficha en la cuadrícula
    if (player == 0) {
        celda_poner_valor(cuadricula[(fila)][(columna)], 0x05);
    } else if (player == 1) {
        celda_poner_valor(cuadricula[(fila)][(columna)], 0x06);
    }
}
*/

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
