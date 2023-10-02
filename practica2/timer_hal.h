// Constante que permite convertir de ticks a microsegundos
unsigned int temporizador_hal_ticks2us =33 /*HALAL*/;

// Funciones
void temporizador_hal_iniciar();
void temporizador_hal_empezar();
uint64_t temporizador_hal_leer();
uint64_t temporizador_hal_parar();