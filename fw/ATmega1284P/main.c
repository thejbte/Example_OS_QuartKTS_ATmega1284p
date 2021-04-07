/**
 * @file main.c
 * @author Julian Bustamante N
 * @brief example system operating  ( QuarkTS ) in  avr (ATmega 1284p)
 * @version 0.0.1
 * @date 2021-04-06
 * @email thejbte@gmail.com
 * @ref https://github.com/kmilo17pet/QuarkTS
 
 *@note
     Output pin : PB0 ,PC1    and PC0 this is a  square wave to  show the 1ms tick  used for the OS
 */

#include <inttypes.h>
#include "QuarkTS.h"
#include "peripheral.h"
#include "task.h"

/* fuses clkdi8 unprogrammed for 8MHz and not 1Mhz
 clock intern RC 8MHz
*/
 
 //#define F_CPU 8000000
  
// global variable to count the number of overflows
volatile uint8_t TickCount;
  

int main(void)
{
    setup_mcu();
    qOS_Setup(NULL, 0.001, App_Task_Idle);
    qOS_Add_Task(&TaskHandlerLed1,App_Task_led1_Callback, qMedium_Priority, 1, qPeriodic, qEnabled, NULL);
    qOS_Add_Task(&TaskHandlerLed2,App_Task_led2_Callback, qMedium_Priority, 0.5, qPeriodic, qEnabled, NULL);
    qOS_Run();
   
  }

