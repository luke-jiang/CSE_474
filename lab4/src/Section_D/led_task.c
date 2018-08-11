//*****************************************************************************
//
// led_task.c - A simple flashing LED task.
//
// Copyright (c) 2012-2017 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.4.178 of the EK-TM4C123GXL Firmware Package.
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "drivers/rgb.h"
#include "drivers/buttons.h"
#include "utils/uartstdio.h"
#include "led_task.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "SSD2119.h"

#include "led_lcd.h"

//*****************************************************************************
//
// The stack size for the LED toggle task.
//
//*****************************************************************************
#define LEDTASKSTACKSIZE        128         // Stack size in words

//*****************************************************************************
//
// The item size and queue size for the LED message queue.
//
//*****************************************************************************
#define LED_ITEM_SIZE           sizeof(uint8_t)
#define LED_QUEUE_SIZE          5

//*****************************************************************************
//
// Default LED toggle delay value. LED toggling frequency is twice this number.
//
//*****************************************************************************
#define LED_TOGGLE_DELAY        250

//*****************************************************************************
//
// The queue that holds messages sent to the LED task.
//
//*****************************************************************************
xQueueHandle g_pLEDQueue;

//
// [G, R, B] range is 0 to 0xFFFF per color.
//
static uint32_t g_pui32Colors[3] = { 0x0000, 0x0000, 0x0000 };
static uint8_t g_ui8ColorsIndx;

extern xSemaphoreHandle g_pUARTSemaphore;


typedef enum {
    OFF_STATE,
    GO_STATE,
    WARN_STATE,
    STOP_STATE
} State;

//*****************************************************************************
//
// This task toggles the user selected LED at a user selected frequency. User
// can make the selections by pressing the left and right buttons.
//
//*****************************************************************************


// green : 1
// blue : 2
// red : 0
static void LEDTask(void *pvParameters) {
    portTickType ui32WakeTime;
    uint32_t ui32LEDToggleDelay;
    uint8_t i8Message;

    // Initialize the LED Toggle Delay to default value.
    ui32LEDToggleDelay = LED_TOGGLE_DELAY;

    // Get the current tick count.
    ui32WakeTime = xTaskGetTickCount();
    
    State curr_state = OFF_STATE;
    State next_state;
    
    // Loop forever.
    while (1) {
      
      switch (curr_state) {
      case OFF_STATE:
        if (xQueueReceive(g_pLEDQueue, &i8Message, 0) == pdPASS) {
          if (i8Message == LEFT_BUTTON) {
            led_ctrl(RED_, 1);
            LCD_PrintString("left button is pressed\n");
            next_state = STOP_STATE;
          } else {
            next_state = OFF_STATE;
          }
        }
      break;
      case GO_STATE:
         if (xQueueReceive(g_pLEDQueue, &i8Message, 0) == pdPASS) {
           if (i8Message == LEFT_BUTTON) {
           led_ctrl(GREEN_, 0);
             LCD_PrintString("left button is pressed\n");
             next_state = OFF_STATE;
           } else if (i8Message == RIGHT_BUTTON) {
             led_ctrl(GREEN_, 0);
             led_ctrl(YELLOW_, 1);
             LCD_PrintString("right button is pressed\n");
             next_state = WARN_STATE;
           } else {
            led_ctrl(GREEN_, 0);
            led_ctrl(RED_, 1);
             next_state = STOP_STATE;
           }
         }
         break;
      case WARN_STATE:
        if (xQueueReceive(g_pLEDQueue, &i8Message, 0) == pdPASS) {
          if (i8Message == LEFT_BUTTON) {
             led_ctrl(YELLOW_, 0);
             LCD_PrintString("left button is pressed\n");
             next_state = OFF_STATE;
          } else {
            led_ctrl(YELLOW_, 0);
            led_ctrl(RED_, 1);
             next_state = STOP_STATE;
          }
        }
        break;
      case STOP_STATE:
         if (xQueueReceive(g_pLEDQueue, &i8Message, 0) == pdPASS) {
          if (i8Message == LEFT_BUTTON) {
             led_ctrl(RED_, 0);
             LCD_PrintString("left button is pressed\n");
             next_state = OFF_STATE;
          } else if (i8Message == RIGHT_BUTTON) {
             LCD_PrintString("left button is pressed\n");
             next_state = STOP_STATE;
          } else {
             led_ctrl(RED_, 0);
             led_ctrl(GREEN_, 1);
             next_state = GO_STATE;
          }
         }
         
        break;
      }
      
      curr_state = next_state;
    }
}
        
     
      

     
     

//*****************************************************************************
//
// Initializes the LED task.
//
//*****************************************************************************
uint32_t LEDTaskInit(void) {
    init_LED(RED_);
    led_off(RED_);
    init_LED(YELLOW_);
    led_off(YELLOW_);
    init_LED(GREEN_);
    led_off(GREEN_);

    // Create a queue for sending messages to the LED task.
    g_pLEDQueue = xQueueCreate(LED_QUEUE_SIZE, LED_ITEM_SIZE);

    // Create the LED task.
    if(xTaskCreate(LEDTask, (const portCHAR *)"LED", LEDTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_LED_TASK, NULL) != pdTRUE)
    {
        return(1);
    }

    //
    // Success.
    //
    return(0);
}