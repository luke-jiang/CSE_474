// CSE 474
// Final Project

// Core Logic

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "led_task.h"
#include "switch_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "driverlib/init.h"
#include "driverlib/SSD2119.h"


#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line) {}
#endif

// This hook is called by FreeRTOS when an stack overflow error is detected.
void vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName){
    while(1);
}


void catapult(void *p) {
    while(1) {
        Touch_ReadX();
        Touch_ReadY();
        long res = Touch_GetCoords();
        long yPos = res & 0xFF;
        long xPos = res >> 16;
        LCD_SetCursor(0, 0);
        LCD_PrintInteger(xPos);
        LCD_PrintChar(' ');
        LCD_PrintInteger(yPos);
        LCD_PrintChar('\n');

        if (yPos > 200 && yPos < 800) {
            char coordstr[10];
            coordstr[0] = '<';
            sprintf(coordstr + 1, "%d", (int) xPos);
            coordstr[4] = ' ';
            sprintf(coordstr + 5, "%d", (int) yPos);
            coordstr[8] = '>';
            coordstr[9] = '\0';
            print_string(coordstr);
            FILE *f = fopen("C:\\Users\\andyk\\Desktop\\data.txt", "w");
            if (!f) LCD_PrintString("File not found");
            fputs(coordstr, f);
            fclose(f);
            for (int i = 0; i < 2000000; i++);
            initialize_PWM(0x437f, 0);
            for (int i = 0; i < 2000000; i++);
            initialize_PWM(0x5a9f, 0);
        }
    }
}

// Initialize FreeRTOS and start the initial set of tasks.
int main(void) {
    LCD_Init(); 
    Touch_Init();
    initialize_clk();
    intialize_UART(104, 11);

    xTaskCreate(catapult, (signed char*)"catapult", 2048, 0, 2, 0);
    vTaskStartScheduler();

    while(1);
}