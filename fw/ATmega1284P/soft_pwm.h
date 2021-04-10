/**
 * @file soft_pwm.h
 * @author Julian Bustamante N
 * @brief software pwm driver
 * @version 0.0.1
 * @date 2021-04-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#ifndef __SOFT_PWM_H_
#define __SOFT_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>  // uint8_t

//SFPWM_
//void escribir_pin(uint8_t pinvalue ){
    // LATbits.RA0 = pinvalue;
//}

#define SFPWM_MAX_COUNTER   65535  /*!<  MAXIMO VALOR TMR1*/ 
#define SFPWM_MAX_PERCENTAGE   100  /*!<  MAXIMO VALOR*/ 

/**
 * @brief pointer to function
 * @param uint8_t output pin value
 * @return void 
 **/
typedef void (*write_pin_fcn)(uint8_t  ) ;

typedef struct  {
    volatile uint16_t count;
    uint16_t dutyc;
    write_pin_fcn write_pin; 
    uint8_t statepin;
    float frequency;
    float basetime;
    float period;
}SFPWM_data_t;

/**
 * @brief 
 * 
 * @param obj pointer to object pwm data
 * @param _write_pin 
 * @param basetiming 
 * @param freq 
 */
void SFPWM_Init(SFPWM_data_t *obj, write_pin_fcn _write_pin, float basetiming ,float freq );

/**
 * @brief 
 * 
 * @param obj 
 */
void SFPWM_Update(SFPWM_data_t *obj);

/**
 * @brief 
 * 
 * @param obj pointer to object pwm data
 * @return uint16_t pwm count ramp
 */
uint16_t SFPWM_GetCount(SFPWM_data_t *obj);

/**
 * @brief get state pin 
 * 
 * @param obj pointer to object pwm data
 * @return uint16_t 0 or 1
 */
uint16_t SFPWM_GetStatePin(SFPWM_data_t *obj);

/**
 * @brief set duty cicle 0-100
 * 
 * @param obj 
 * @param duty 
 */
void SFPWM_SetDuty(SFPWM_data_t *obj , uint16_t duty);



#ifdef __cplusplus
}
#endif
#endif
