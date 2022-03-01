/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "CDH.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
typedef StaticTimer_t osStaticTimerDef_t;
typedef StaticSemaphore_t osStaticMutexDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
uint32_t defaultTaskBuffer[ 256 ];
osStaticThreadDef_t defaultTaskControlBlock;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .cb_mem = &defaultTaskControlBlock,
  .cb_size = sizeof(defaultTaskControlBlock),
  .stack_mem = &defaultTaskBuffer[0],
  .stack_size = sizeof(defaultTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CAN_Task */
osThreadId_t CAN_TaskHandle;
uint32_t CAN_TaskBuffer[ 256 ];
osStaticThreadDef_t CAN_TaskControlBlock;
const osThreadAttr_t CAN_Task_attributes = {
  .name = "CAN_Task",
  .cb_mem = &CAN_TaskControlBlock,
  .cb_size = sizeof(CAN_TaskControlBlock),
  .stack_mem = &CAN_TaskBuffer[0],
  .stack_size = sizeof(CAN_TaskBuffer),
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for GPS_Task */
osThreadId_t GPS_TaskHandle;
uint32_t GPS_TaskBuffer[ 256 ];
osStaticThreadDef_t GPS_TaskControlBlock;
const osThreadAttr_t GPS_Task_attributes = {
  .name = "GPS_Task",
  .cb_mem = &GPS_TaskControlBlock,
  .cb_size = sizeof(GPS_TaskControlBlock),
  .stack_mem = &GPS_TaskBuffer[0],
  .stack_size = sizeof(GPS_TaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for RAS_Blob_Task */
osThreadId_t RAS_Blob_TaskHandle;
uint32_t RAS_Blob_TaskBuffer[ 256 ];
osStaticThreadDef_t RAS_Blob_TaskControlBlock;
const osThreadAttr_t RAS_Blob_Task_attributes = {
  .name = "RAS_Blob_Task",
  .cb_mem = &RAS_Blob_TaskControlBlock,
  .cb_size = sizeof(RAS_Blob_TaskControlBlock),
  .stack_mem = &RAS_Blob_TaskBuffer[0],
  .stack_size = sizeof(RAS_Blob_TaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for OBC_Blob_Task */
osThreadId_t OBC_Blob_TaskHandle;
uint32_t OBC_Blob_TaskBuffer[ 256 ];
osStaticThreadDef_t OBC_Blob_TaskControlBlock;
const osThreadAttr_t OBC_Blob_Task_attributes = {
  .name = "OBC_Blob_Task",
  .cb_mem = &OBC_Blob_TaskControlBlock,
  .cb_size = sizeof(OBC_Blob_TaskControlBlock),
  .stack_mem = &OBC_Blob_TaskBuffer[0],
  .stack_size = sizeof(OBC_Blob_TaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Storage_Task */
osThreadId_t Storage_TaskHandle;
uint32_t Storage_TaskBuffer[ 256 ];
osStaticThreadDef_t Storage_TaskControlBlock;
const osThreadAttr_t Storage_Task_attributes = {
  .name = "Storage_Task",
  .cb_mem = &Storage_TaskControlBlock,
  .cb_size = sizeof(Storage_TaskControlBlock),
  .stack_mem = &Storage_TaskBuffer[0],
  .stack_size = sizeof(Storage_TaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for PktBuilder_Task */
osThreadId_t PktBuilder_TaskHandle;
uint32_t PktBuilder_TaskBuffer[ 256 ];
osStaticThreadDef_t PktBuilder_TaskControlBlock;
const osThreadAttr_t PktBuilder_Task_attributes = {
  .name = "PktBuilder_Task",
  .cb_mem = &PktBuilder_TaskControlBlock,
  .cb_size = sizeof(PktBuilder_TaskControlBlock),
  .stack_mem = &PktBuilder_TaskBuffer[0],
  .stack_size = sizeof(PktBuilder_TaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Radio_Task */
osThreadId_t Radio_TaskHandle;
uint32_t Radio_TaskBuffer[ 256 ];
osStaticThreadDef_t Radio_TaskControlBlock;
const osThreadAttr_t Radio_Task_attributes = {
  .name = "Radio_Task",
  .cb_mem = &Radio_TaskControlBlock,
  .cb_size = sizeof(Radio_TaskControlBlock),
  .stack_mem = &Radio_TaskBuffer[0],
  .stack_size = sizeof(Radio_TaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Stimulus_Task */
osThreadId_t Stimulus_TaskHandle;
uint32_t Stimulus_TaskBuffer[ 256 ];
osStaticThreadDef_t Stimulus_TaskControlBlock;
const osThreadAttr_t Stimulus_Task_attributes = {
  .name = "Stimulus_Task",
  .cb_mem = &Stimulus_TaskControlBlock,
  .cb_size = sizeof(Stimulus_TaskControlBlock),
  .stack_mem = &Stimulus_TaskBuffer[0],
  .stack_size = sizeof(Stimulus_TaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for CAN_Queue */
osMessageQueueId_t CAN_QueueHandle;
uint8_t CAN_QueueBuffer[ 10 * sizeof( CAN_Event_t ) ];
osStaticMessageQDef_t CAN_QueueControlBlock;
const osMessageQueueAttr_t CAN_Queue_attributes = {
  .name = "CAN_Queue",
  .cb_mem = &CAN_QueueControlBlock,
  .cb_size = sizeof(CAN_QueueControlBlock),
  .mq_mem = &CAN_QueueBuffer,
  .mq_size = sizeof(CAN_QueueBuffer)
};
/* Definitions for Radio_Queue */
osMessageQueueId_t Radio_QueueHandle;
uint8_t Radio_QueueBuffer[ 8 * sizeof( radio_event_t ) ];
osStaticMessageQDef_t Radio_QueueControlBlock;
const osMessageQueueAttr_t Radio_Queue_attributes = {
  .name = "Radio_Queue",
  .cb_mem = &Radio_QueueControlBlock,
  .cb_size = sizeof(Radio_QueueControlBlock),
  .mq_mem = &Radio_QueueBuffer,
  .mq_size = sizeof(Radio_QueueBuffer)
};
/* Definitions for GPS_Queue */
osMessageQueueId_t GPS_QueueHandle;
uint8_t GPS_QueueBuffer[ 2 * sizeof( gps_event_t ) ];
osStaticMessageQDef_t GPS_QueueControlBlock;
const osMessageQueueAttr_t GPS_Queue_attributes = {
  .name = "GPS_Queue",
  .cb_mem = &GPS_QueueControlBlock,
  .cb_size = sizeof(GPS_QueueControlBlock),
  .mq_mem = &GPS_QueueBuffer,
  .mq_size = sizeof(GPS_QueueBuffer)
};
/* Definitions for OBC_Fragment_Queue */
osMessageQueueId_t OBC_Fragment_QueueHandle;
uint8_t OBC_Fragment_QueueBuffer[ 8 * sizeof( fragment_t ) ];
osStaticMessageQDef_t OBC_Fragment_QueueControlBlock;
const osMessageQueueAttr_t OBC_Fragment_Queue_attributes = {
  .name = "OBC_Fragment_Queue",
  .cb_mem = &OBC_Fragment_QueueControlBlock,
  .cb_size = sizeof(OBC_Fragment_QueueControlBlock),
  .mq_mem = &OBC_Fragment_QueueBuffer,
  .mq_size = sizeof(OBC_Fragment_QueueBuffer)
};
/* Definitions for RAS_Fragment_Queue */
osMessageQueueId_t RAS_Fragment_QueueHandle;
uint8_t RAS_Fragment_QueueBuffer[ 14 * sizeof( fragment_t ) ];
osStaticMessageQDef_t RAS_Fragment_QueueControlBlock;
const osMessageQueueAttr_t RAS_Fragment_Queue_attributes = {
  .name = "RAS_Fragment_Queue",
  .cb_mem = &RAS_Fragment_QueueControlBlock,
  .cb_size = sizeof(RAS_Fragment_QueueControlBlock),
  .mq_mem = &RAS_Fragment_QueueBuffer,
  .mq_size = sizeof(RAS_Fragment_QueueBuffer)
};
/* Definitions for OBC_Blob_Queue */
osMessageQueueId_t OBC_Blob_QueueHandle;
uint8_t OBC_Blob_QueueBuffer[ 4 * sizeof( obc_blob_t ) ];
osStaticMessageQDef_t OBC_Blob_QueueControlBlock;
const osMessageQueueAttr_t OBC_Blob_Queue_attributes = {
  .name = "OBC_Blob_Queue",
  .cb_mem = &OBC_Blob_QueueControlBlock,
  .cb_size = sizeof(OBC_Blob_QueueControlBlock),
  .mq_mem = &OBC_Blob_QueueBuffer,
  .mq_size = sizeof(OBC_Blob_QueueBuffer)
};
/* Definitions for RAS_Blob_Queue */
osMessageQueueId_t RAS_Blob_QueueHandle;
uint8_t RAS_Blob_QueueBuffer[ 14 * sizeof( ras_blob_t ) ];
osStaticMessageQDef_t RAS_Blob_QueueControlBlock;
const osMessageQueueAttr_t RAS_Blob_Queue_attributes = {
  .name = "RAS_Blob_Queue",
  .cb_mem = &RAS_Blob_QueueControlBlock,
  .cb_size = sizeof(RAS_Blob_QueueControlBlock),
  .mq_mem = &RAS_Blob_QueueBuffer,
  .mq_size = sizeof(RAS_Blob_QueueBuffer)
};
/* Definitions for OBC_Timer */
osTimerId_t OBC_TimerHandle;
osStaticTimerDef_t OBC_TimerControlBlock;
const osTimerAttr_t OBC_Timer_attributes = {
  .name = "OBC_Timer",
  .cb_mem = &OBC_TimerControlBlock,
  .cb_size = sizeof(OBC_TimerControlBlock),
};
/* Definitions for RAS_Timer */
osTimerId_t RAS_TimerHandle;
osStaticTimerDef_t RAS_TimerControlBlock;
const osTimerAttr_t RAS_Timer_attributes = {
  .name = "RAS_Timer",
  .cb_mem = &RAS_TimerControlBlock,
  .cb_size = sizeof(RAS_TimerControlBlock),
};
/* Definitions for Radio_Timer */
osTimerId_t Radio_TimerHandle;
osStaticTimerDef_t Radio_TimerControlBlock;
const osTimerAttr_t Radio_Timer_attributes = {
  .name = "Radio_Timer",
  .cb_mem = &Radio_TimerControlBlock,
  .cb_size = sizeof(Radio_TimerControlBlock),
};
/* Definitions for GPS_Timer */
osTimerId_t GPS_TimerHandle;
osStaticTimerDef_t GPS_TimerControlBlock;
const osTimerAttr_t GPS_Timer_attributes = {
  .name = "GPS_Timer",
  .cb_mem = &GPS_TimerControlBlock,
  .cb_size = sizeof(GPS_TimerControlBlock),
};
/* Definitions for Config_Mutex */
osMutexId_t Config_MutexHandle;
osStaticMutexDef_t Config_MutexControlBlock;
const osMutexAttr_t Config_Mutex_attributes = {
  .name = "Config_Mutex",
  .cb_mem = &Config_MutexControlBlock,
  .cb_size = sizeof(Config_MutexControlBlock),
};
/* Definitions for UART_Mutex */
osMutexId_t UART_MutexHandle;
osStaticMutexDef_t UART_MutexControlBlock;
const osMutexAttr_t UART_Mutex_attributes = {
  .name = "UART_Mutex",
  .cb_mem = &UART_MutexControlBlock,
  .cb_size = sizeof(UART_MutexControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void CAN_TaskFun(void *argument);
extern void GPS_TaskFun(void *argument);
extern void RAS_Blob_TaskFun(void *argument);
extern void OBC_Blob_TaskFun(void *argument);
extern void Storage_TaskFun(void *argument);
extern void PktBuilder_TaskFun(void *argument);
extern void Radio_TaskFun(void *argument);
extern void Stimulus_TaskFun(void *argument);
extern void OBC_TimerFun(void *argument);
extern void RAS_TimerFun(void *argument);
extern void Radio_TimerFun(void *argument);
extern void GPS_TimerFun(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN PREPOSTSLEEP */
__weak void PreSleepProcessing(uint32_t ulExpectedIdleTime)
{
/* place for user code */
}

__weak void PostSleepProcessing(uint32_t ulExpectedIdleTime)
{
/* place for user code */
}
/* USER CODE END PREPOSTSLEEP */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of Config_Mutex */
  Config_MutexHandle = osMutexNew(&Config_Mutex_attributes);

  /* creation of UART_Mutex */
  UART_MutexHandle = osMutexNew(&UART_Mutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of OBC_Timer */
  OBC_TimerHandle = osTimerNew(OBC_TimerFun, osTimerPeriodic, NULL, &OBC_Timer_attributes);

  /* creation of RAS_Timer */
  RAS_TimerHandle = osTimerNew(RAS_TimerFun, osTimerPeriodic, NULL, &RAS_Timer_attributes);

  /* creation of Radio_Timer */
  Radio_TimerHandle = osTimerNew(Radio_TimerFun, osTimerPeriodic, NULL, &Radio_Timer_attributes);

  /* creation of GPS_Timer */
  GPS_TimerHandle = osTimerNew(GPS_TimerFun, osTimerPeriodic, NULL, &GPS_Timer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of CAN_Queue */
  CAN_QueueHandle = osMessageQueueNew (10, sizeof(CAN_Event_t), &CAN_Queue_attributes);

  /* creation of Radio_Queue */
  Radio_QueueHandle = osMessageQueueNew (8, sizeof(radio_event_t), &Radio_Queue_attributes);

  /* creation of GPS_Queue */
  GPS_QueueHandle = osMessageQueueNew (2, sizeof(gps_event_t), &GPS_Queue_attributes);

  /* creation of OBC_Fragment_Queue */
  OBC_Fragment_QueueHandle = osMessageQueueNew (8, sizeof(fragment_t), &OBC_Fragment_Queue_attributes);

  /* creation of RAS_Fragment_Queue */
  RAS_Fragment_QueueHandle = osMessageQueueNew (14, sizeof(fragment_t), &RAS_Fragment_Queue_attributes);

  /* creation of OBC_Blob_Queue */
  OBC_Blob_QueueHandle = osMessageQueueNew (4, sizeof(obc_blob_t), &OBC_Blob_Queue_attributes);

  /* creation of RAS_Blob_Queue */
  RAS_Blob_QueueHandle = osMessageQueueNew (14, sizeof(ras_blob_t), &RAS_Blob_Queue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of CAN_Task */
  CAN_TaskHandle = osThreadNew(CAN_TaskFun, NULL, &CAN_Task_attributes);

  /* creation of GPS_Task */
  GPS_TaskHandle = osThreadNew(GPS_TaskFun, NULL, &GPS_Task_attributes);

  /* creation of RAS_Blob_Task */
  RAS_Blob_TaskHandle = osThreadNew(RAS_Blob_TaskFun, NULL, &RAS_Blob_Task_attributes);

  /* creation of OBC_Blob_Task */
  OBC_Blob_TaskHandle = osThreadNew(OBC_Blob_TaskFun, NULL, &OBC_Blob_Task_attributes);

  /* creation of Storage_Task */
  Storage_TaskHandle = osThreadNew(Storage_TaskFun, NULL, &Storage_Task_attributes);

  /* creation of PktBuilder_Task */
  PktBuilder_TaskHandle = osThreadNew(PktBuilder_TaskFun, NULL, &PktBuilder_Task_attributes);

  /* creation of Radio_Task */
  Radio_TaskHandle = osThreadNew(Radio_TaskFun, NULL, &Radio_Task_attributes);

  /* creation of Stimulus_Task */
  Stimulus_TaskHandle = osThreadNew(Stimulus_TaskFun, NULL, &Stimulus_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

