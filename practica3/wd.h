
#ifndef WD_H
#define WD_H
#include <LPC210X.H>
#include "SWI.h"
void WD_hal_init(int sec);
void WD_hal_feed(void);
void WD_hal_test(void);
#endif
