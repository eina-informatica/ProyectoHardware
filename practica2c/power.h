#include <LPC210x.H>

#ifndef POWER_H
#define POWER_H

void power_hal_wait(void);
void power_hal_deep_sleep(void); 
extern void Switch_to_PLL(void);

#endif
