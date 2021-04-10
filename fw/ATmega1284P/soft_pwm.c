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

#include "soft_pwm.h"
#include <stdlib.h>
#include <stdio.h>


static int16_t SFPWM_Rounding(float num);

void SFPWM_Init(SFPWM_data_t *obj, write_pin_fcn _write_pin, float basetiming ,float freq )
{   obj->write_pin = _write_pin ; 
    obj->frequency = freq;
    obj->period = (1 / obj->frequency);  //0.1
    obj->basetime = basetiming;   //0.001
    obj->count = 0;
}

void SFPWM_Update(SFPWM_data_t *obj){

    ++obj->count;

    if (   SFPWM_Rounding( (obj->period *(1/obj->basetime)) )   == obj->count ){  // overflow period
        obj->count = 0;  // overflow
    }

    obj->statepin = ( obj->count  < obj->dutyc ) ? 1 : 0 ;
    if( obj->write_pin != NULL){
        obj->write_pin( obj->statepin ); // 10 < 127  
    }
}

uint16_t SFPWM_GetCount(SFPWM_data_t *obj){
    return obj->count;
}

uint16_t SFPWM_GetStatePin(SFPWM_data_t *obj){
    return obj->statepin;
}

void SFPWM_SetDuty(SFPWM_data_t *obj , uint16_t duty){
    //obj->dutyc = (duty*obj->period  ) / SFPWM_MAX_PERCENTAGE;
    obj->dutyc = SFPWM_Rounding(  ((duty*(obj->period * (1/obj->basetime)) ) / SFPWM_MAX_PERCENTAGE)  )  ;
    //printf(" 1/bt=%d seg=%d \n",(uint16_t)(1/obj->basetime) , (obj->period * (1/obj->basetime)));
}

static int16_t SFPWM_Rounding(float num){
    return num < 0 ? num  - 0.5 : num + 0.5;
}