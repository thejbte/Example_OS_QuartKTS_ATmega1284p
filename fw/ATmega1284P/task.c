
/**
 * @file task.c
 * @author Julian Bustamante N
 * @brief  task for OS
 *@date 2021-04-06
 */
 
 
#include "QuarkTS.h"
#include "task.h"
#include <avr/io.h>

/*tarea que se ejcuta cuando las demás no lo lo hacen*/
void App_Task_Idle(qEvent_t e){
	 // PORTC ^= (1 << 0);
}


  /*tarea que prende un led*/
void App_Task_led1_Callback(qEvent_t arg){
    PORTB   ^= (1 << 0);
}

void App_Task_led2_Callback(qEvent_t arg){
    PORTC   ^= (1 << 1);  // pc1
}


