#ifndef SWI_H
#define SWI_H
#include <stdint.h>

uint32_t __swi(1) read_irq_bit(void);
void __swi(0xFF) enable_irq(void);
void __swi(0xFE) disable_irq(void);
void __swi(0xFD) disable_fiq(void);
//void __swi(0xFC) disable_irq_fiq(void);

#endif
