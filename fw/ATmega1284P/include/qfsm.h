/*This file is part of the QuarkTS distribution.*/
#ifndef QFSM_H
    #define QFSM_H

    #include "qtypes.h"
    #include "qqueues.h"
    #include "qstimers.h"

    #ifdef __cplusplus
    extern "C" {
    #endif

    typedef enum {qSM_BEFORE_ANY = -32767, qSM_EXIT_NULL = -32766, qSM_EXIT_FAILURE = -32765, qSM_EXIT_SUCCESS = -32764} qSM_Status_t;
    #define QSM_EXIT_SUCCESS        ( qSM_EXIT_SUCCESS )
    #define QSM_EXIT_FAILURE        ( qSM_EXIT_FAILURE )
    #define QSM_EXIT_NULL           ( qSM_EXIT_NULL )
    #define QSM_BEFORE_ANY          ( qSM_BEFORE_ANY )
    #define _qSM_Handler_t struct _qSM_PublicData_s * 

    typedef qUINT32_t qSM_Signal_t;

    #define QSM_SIGNAL_RANGE_MIN        ( (qSM_Signal_t)0u )
    #define QSM_SIGNAL_RANGE_MAX        ( (qSM_Signal_t)0xFFFFFFFCu )

    #define QSM_SIGNAL_NONE             ( (qSM_Signal_t)0xFFFFFFFFu )
    #define QSM_SIGNAL_ENTRY            ( (qSM_Signal_t)0xFFFFFFFEu )
    #define QSM_SIGNAL_EXIT             ( (qSM_Signal_t)0xFFFFFFFDu )
    #define QSM_SIGNAL_TIMEOUT(index)   ( (qSM_Signal_t)0xFFFFFFFCu - (qSM_Signal_t)(Q_FSM_MAX_TIMEOUTS-1) + (qSM_Signal_t)(index) )   
    #define QSM_SIGNAL_USER             ( QSM_SIGNAL_RANGE_MIN )
  

    typedef struct _qSM_PublicData_s{
        /* NextState: (Read/Write) 
        Next state to be performed after this state finish
        */
        qSM_Status_t (*NextState)(_qSM_Handler_t arg);
        /* PreviousState: (Read Only)
        Last state seen in the flow chart
        */
        qSM_Status_t (*PreviousState)(_qSM_Handler_t arg);
        /* LastState: (Read Only) 
        The last state executed
        */        
        qSM_Status_t (*LastState)(_qSM_Handler_t arg);
        /* Data: (Read Only)
        State-machine associated data.
        Note: If the FSM is running as a task, the associated event data can be 
        queried throught the "Data" field. (cast to qEvent_t is mandatory)
        */
        void *Data;
        /* Signal: (Read Only)
        Received signal from the transition table queue.
        QSM_SIGNAL_NONE if no table or signal available.
        QSM_SIGNAL_ENTRY When the current state has just entry
        QSM_SIGNAL_EXIT when the current state is exitimg after a transition
        */
        qSM_Signal_t Signal;
        /* PreviousReturnStatus: (Read Only)
        The return status of <PreviousState>
        */
        qSM_Status_t PreviousReturnStatus;
        /* LastReturnStatus: (Read Only)
        The return status of <LastState>
        */        
        qSM_Status_t LastReturnStatus;
        /* Parent: (Read/Write)  
        The parent FSM
        */
        void *Parent;
    }_qSM_PublicData_t;

    typedef _qSM_PublicData_t* qSM_Handler_t;    
    typedef qSM_Status_t (*qSM_State_t)(qSM_Handler_t arg); 
    typedef void (*qSM_SurroundingState_t)(qSM_Handler_t arg); 
    typedef qBool_t (*qSM_SignalAction)( qSM_Handler_t arg );

    typedef struct{
        qSM_State_t xCurrentState;                      /*< The current state that the FSM occupies*/
        qSM_Signal_t Signal;                            /*< The event-signal used to produce the transition*/
        qSM_State_t xNextState;                         /*< The next state that the FSM will occupy after the transition. NULL if not used*/
        qSM_SignalAction SignalAction;                  /*< The action performed by the signal on the current transition. NULL to disable.*/
        void *xToTargetHandle;                          /*< (Only in hierarchycal FSM) The handle to the target child. */
        qSM_State_t xToTargetState;                     /*< (Only in hierarchycal FSM) The next state that the child FSM will occupy after the transition. NULL if not used*/
    }qSM_Transition_t;

    typedef struct{
        /*This data should be handled only using the provided API*/
        struct _qSM_TransitionTable_Private_s{
            size_t NumberOfEntries;                     /*< The number of entries inside the <Transitions> array */         
            qSM_Transition_t *Transitions;              /*< Points to the table entries, an array of type qSM_Transition_t[]*/
        }qPrivate;
    }qSM_TransitionTable_t;

    typedef struct{
        qSM_State_t xState;                             /*< in witch state the timeout will be installed.*/
        qTime_t xTimeout;                               /*< the time value to be used as Timeout.*/
        size_t index;                                   /*< the for the timeout. */
    }qSM_TimeoutStateDefinition_t;

    typedef struct{
        qSM_TimeoutStateDefinition_t *spec;             /*< a pointer to the state-timeout lookup table*/
        qSTimer_t builtin_timeout[ Q_FSM_MAX_TIMEOUTS ];/*< the built-in timeouts*/
        size_t n;                                       /*< the number of entries inside the <spec> field*/
    }qSM_TimeoutSpec_t;

    /* Please don't access any members of this structure directly */
    typedef struct _qSM_s{
        struct _qSM_Private_s{
            qSM_SurroundingState_t Surrounding;         /*< An additional state to handle actions after and before any normal state. */
            qSM_State_t InitState;                      /*< The initial state*/
            qSM_TransitionTable_t *TransitionTable;     /*< A pointer to the transition table.*/
            void *Owner;                                /*< A pointer to the owner task */
            struct{             
                struct _qSM_s *head;                    /*< Composite state pointer. head : points to the head of the nested fsm list .*/    
                struct _qSM_s *next;                    /*< Composite state pointer. head : points to the next same-level fsm.*/
                qSM_State_t rootState;                  /*< The state where this fsm should be active*/
            }Composite; 
            qQueue_t *SignalQueue;                       /*< The fsm signal queue object. */
            _qSM_PublicData_t xPublic;                  /*< The external-manipulable members of the fsm. */
            qSM_TimeoutSpec_t *TimeSpec;                /*< A pointer to the timeout specification object*/
            qBool_t Active;                             /*< A flag indicating whether the fsm should run in a hierarchical environment*/          
        }qPrivate;
    }qSM_t;

    #define QSM_STATE_UNDEFINED     ( &_qStateMachine_UndefinedStateCallback )
    #define QSM_STATE_TO_SAME       ( &_qStateMachine_RecursiveStateCallback )

    typedef enum{ /*FSM Attribute Flags definition*/
        qSM_RESTART,                                    /*< Restart the FSM. */
        qSM_CLEAR_STATE_FIRST_ENTRY_FLAG,               /*< Clear the entry flag for the current state if the NextState field doesn't change. */
        qSM_SURROUNDING_STATE,                          /*< To set the surrounding state*/       
        qSM_UNINSTALL_TRANSTABLE,                       /*< To unistall the transition table if available*/      
        qSM_CHANGE_INITSTATE                            /*< To change the init state*/
    }qSM_Attribute_t; 

    qBool_t qStateMachine_Setup( qSM_t * const obj, qSM_State_t InitState, qSM_SurroundingState_t Surrounding );
    void qStateMachine_Run( qSM_t * const root, void *Data );

    void qStateMachine_Attribute( qSM_t * const obj, const qSM_Attribute_t Flag , qSM_State_t  s, qSM_SurroundingState_t Surrounding  );
    qBool_t qStateMachine_SignalQueueSetup( qSM_t * const obj, qQueue_t *xQueue, qSM_Signal_t *AxSignals, size_t MaxSignals );
    qBool_t qStateMachine_TransitionTableInstall( qSM_t * const obj, qSM_TransitionTable_t *table, qSM_Transition_t *entries, size_t NoOfEntries );
    qBool_t qStateMachine_SweepTransitionTable( qSM_t * const obj, qSM_Signal_t xSignal );
    qBool_t qStateMachine_SendSignal( qSM_t * const obj, qSM_Signal_t xSignal, qBool_t isUrgent );
    qSM_Handler_t qStateMachine_Get_Handler( qSM_t * const obj );
    void qStateMachine_Set_Parent( qSM_t * const Child, qSM_t * const Parent );
    
    qBool_t qStateMachine_Set_CompositeState( qSM_t * const parent, qSM_State_t state, qSM_t * const child );

    qSM_Status_t _qStateMachine_UndefinedStateCallback( qSM_Handler_t h );
    qSM_Status_t _qStateMachine_RecursiveStateCallback( qSM_Handler_t h );

    
    qBool_t qStateMachine_TimeoutSpecInstall( qSM_t * const obj,  qSM_TimeoutSpec_t *ts, qSM_TimeoutStateDefinition_t *tdef, size_t n );
    qBool_t qStateMachine_SetTimeout( qSM_t * const obj, const qIndex_t xTimeout, const qTime_t time );

    #ifdef __cplusplus
    }
    #endif

#endif 
