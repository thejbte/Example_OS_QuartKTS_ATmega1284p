
/**
 * @file interrupt.c
 * @author Julian Bustamante N
 * @brief  IRQ handler
 * @date 2021-04-06
 */
 
#include <avr/interrupt.h>
#include "QuarkTS.h"

extern volatile uint8_t TickCount;


// TIMER0 overflow interrupt service routine
// called whenever timer0 - CTC mode overflows
ISR(TIMER0_COMPA_vect )    //TIMER0_OVF_vect)
{   
   qClock_SysTick();  // esta es la que se necesita para que el OS funcione
    TickCount++; 
    PORTC ^= (1 << 0);
}
