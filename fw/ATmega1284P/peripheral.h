/**
 * @file peripheral.h
 * @author Julian Bustamante N
 * @brief  peripherals config MCU
 * @date 2021-04-06
 */
 #ifndef __PERIPHERAL_H_
 #define __PERIPHERAL_H_
 
 
 #ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
    
    
void timer0_init();

void setup_mcu();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  //  __PERIPHERAL_H_