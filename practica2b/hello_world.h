#include <stddef.h>
#include "io_reserva.h"
#include "gpio_hal.h"
#include "alarmas.h"

#ifndef HELLO_WORLD_H
#define HELLO_WORLD_H

static int pin, size, tick_count, tick_count_decimas;

void hello_world_inicializar(void);
void hello_world_tick_tack(void);
void hello_world_tratar_evento(void);

#endif
