#include "wd.h"
void WD_hal_init(int sec) {
 WDTC = sec * 15000000 / 4;
 WDMOD = 3;//HAce que se resetee el programa
}

void WD_hal_feed(void) {
 disable_irq();
 WDFEED = 0xaa;
 WDFEED = 0x55;
 enable_irq();
}
void WD_hal_test(void) {
 while(1);
 }
