#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include <LPC210x.H>
#include <stddef.h>
#include "hello_world.h"
#include "timer_drv.h"
#include "gestor_energia.h"
#include "botones.h"
#include "visualizar.h"
#include "linea_serie_drv.h"
#include "wd.h"
#include "alarmas.h"
#include "juego.h"
#include "io_reserva.h"

void planificador(void);

#endif
