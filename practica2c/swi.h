#ifndef SWI_H
#define SWI_H

#include <LPC210X.H>

/*void PAbt_Handler_function(void) ();
void DAbt_Handler_function(void) ();*/

unsigned int __swi(0x01) read_irq_bit(void);

void __swi(0xFF) enable_irq(void);
void __swi(0xFE) disable_irq(void);
void __swi(0xFD) disable_fiq(void);
//void __swi(0xFC) disable_irq_fiq(void);

#endif
