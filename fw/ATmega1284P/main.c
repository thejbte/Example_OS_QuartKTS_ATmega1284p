/**
 * @file main.c
 * @author Julian Bustamante N
 * @brief example system operating  ( QuarkTS ) in  avr (ATmega 1284p)
 * @version 0.1.0
 * @date 2021-04-06
 * @email thejbte@gmail.com
 * @ref https://github.com/kmilo17pet/QuarkTS
 
 *@note
     Output pin : PB0 ,PB1(pwm sf),PC1    and PC0 this is a  square wave to  show the 1ms tick  used for the OS
 */

#include <inttypes.h>
#include "QuarkTS.h"
#include "peripheral.h"
#include "task.h"
#include "soft_pwm.h"
#include <avr/io.h>

/* fuses clkdi8 unprogrammed for 8MHz and not 1Mhz
 clock intern RC 8MHz
*/
 
 //#define F_CPU 8000000
  
// global variable to count the number of overflows
volatile uint8_t TickCount;
 
 void WrapperGpio(uint8_t pinvalue);

SFPWM_data_t SFPWM_data;

int main(void)
{
    setup_mcu();
   SFPWM_Init(&SFPWM_data ,WrapperGpio , 0.001 ,10.0);
   SFPWM_SetDuty( &SFPWM_data , 10);
    qOS_Setup(NULL, 0.001, App_Task_Idle);
    qOS_Add_Task(&TaskHandlerLed1,App_Task_led1_Callback, qMedium_Priority, 1, qPeriodic, qEnabled, NULL);
    qOS_Add_Task(&TaskHandlerLed2,App_Task_led2_Callback, qMedium_Priority, 0.5, qPeriodic, qEnabled, NULL);
    qOS_Run();

  }

  
  void WrapperGpio(uint8_t pinvalue){
      if( pinvalue == 1){
	    PORTC = (1 << 0); //PC0  on
	 /*prendo pin*/
	 }else {
	    PORTC = (0 << 0);   //PC0  off
	    /*apago*/
	  }
     }
