
#include "power.h"                 
   /* LPC210x definitions */
// Set the processor into power down state 
// The watchdog cannot wake up the processor from power down
void PM_power_down(void)  {
  EXTWAKE = 1; // EXTINT0 will awake the processor
	PCON |= 0x01; 
	//PCON |= 0x02; 
  //Switch_to_PLL();
}
