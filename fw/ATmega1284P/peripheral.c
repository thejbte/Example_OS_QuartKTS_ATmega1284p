
/**
 * @file peripheral.c
 * @author Julian Bustamante N
 * @brief  peripherals config MCU
 * @date 2021-04-06
 */
 
#include "peripheral.h"
#include <avr/io.h>
#include <avr/interrupt.h>

// initialize timer, interrupt and variable
void timer0_init()
{
   //funciona con https://eleccelerator.com/avr-timer-calculator/   Fosc = fclk /presc     fclk /presc =ocr*fosc   fclk / fosc*presc =ocr  
   // con esta formula sacada empiricamente da ocr = fclk / fosc*presc      =8000000 / 64*1000Hz
     //1000 Hz = (8000000 / (   2*64*(   61.5 +1) )   -> 1ms tick    con esta no da
     //fosc =  (  fclock/ (2*Presc*(OCRx  +  1)    )
 
     // Clear Timer on Compare (CTC) mode ; 
     TCCR0A = (1 << WGM01) ;   
     TCCR0B   |=   (1 << CS01)  |  (1 << CS00) ;   // Set prescaler to 64 


     // set Output Compare Register to (250 - 1) ticks
     OCR0A = 125;  //61.5  ~~ 62
     
     // Set Timer Interrupt Mask Register to
     // Clear Timer on Compare channel A for timer 0
     TIMSK0  |= (1 << OCIE0A ) ;
     CLKPR = 0;   // clock mode normal  Clock Division Factor
     sei();
}

void setup_mcu(){
      DDRC |= (1 << 1) | (1 << 0);  // pC0  & pc1 as  output
      DDRB |= (1 << 1) | (1 << 0);   // pB0  as output

      // initialize timer
      timer0_init();
   
   }