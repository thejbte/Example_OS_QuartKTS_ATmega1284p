/**
 * @file task.h
 * @author Julian Bustamante N
 * @brief  task for OS
 *@date 2021-04-06
 */
 
 #ifndef __TASK_H_
 #define __TASK_H_
 
 
  #ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */
    
    
qTask_t TaskHandlerLed1;
qTask_t TaskHandlerLed2;

void App_Task_Idle(qEvent_t e);

  /*tarea que prende un led*/
void App_Task_led1_Callback(qEvent_t arg);
void App_Task_led2_Callback(qEvent_t arg);

    
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  //  __TASK_H_