#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
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


#define RED                     0x80
#define BLUE                    0x40
#define GREEN                   0x20
#define RGB                     0x0E
/*
#define TIMER0_CTL_R            (*((volatile unsigned long *)0x4003000C))
#define TIMER0_CFG_R            (*((volatile unsigned long *)0x40030000))
#define TIMER0_TAMR_R           (*((volatile unsigned long *)0x40030004))
#define TIMER0_TAILR_R          (*((volatile unsigned long *)0x40030028))
#define TIMER0_RIS_R            (*((volatile unsigned long *)0x4003001C))
#define TIMER0_ICR_R            (*((volatile unsigned long *)0x40030024))
*/
void LED_init ();
void timer_init ();
void green (void* p);
void blue (void* p);
void red (void* p);
void delay ();

int i;

void vApplicationStackOverflowHook () {
  while(1){}
}

// blink the Red LED in every 50 delay
void red (void* p) {
  while (1) {
   
    GPIO_PORTC_DATA_R |= RED;
      
         
    vTaskDelay(50);
    GPIO_PORTC_DATA_R = 0x0;
    vTaskDelay(50);
  }
}

// blink the Blue LED in every 50 delay
void blue (void* p) {
  while (1) {
  
    
      GPIO_PORTC_DATA_R |= BLUE;
     
    vTaskDelay(100);
    GPIO_PORTC_DATA_R = 0x0;
    vTaskDelay(100);
  }
}

// blink the Green LED in every 50 delay
void green (void* p) {
  while (1) {
   
      GPIO_PORTC_DATA_R |= GREEN;
      
         
    vTaskDelay(300);
    GPIO_PORTC_DATA_R = 0x0;
    vTaskDelay(300);
  }
}

// configure the port and pins for the LEDs
void LED_init () { volatile unsigned long delay;
  SYSCTL_RCGC2_R |= (1<<2);               // 1) activate clock for Port C
  delay = SYSCTL_RCGC2_R;               // allow time for clock to start
                                        // no need to unlock PA5,PA6,PA7
  GPIO_PORTC_PCTL_R &= ~0xFFF00000;     // regular GPIO
  GPIO_PORTC_AMSEL_R &= ~0xE0;          // 4) disable analog function on PC5,PC6,PC7
  GPIO_PORTC_DIR_R |= 0xE0;             // 5) set direction to output 
  GPIO_PORTC_AFSEL_R &= ~0xE0;          // 6) regular port function
  GPIO_PORTC_DEN_R |= 0xE0;             // 7) enable digital port
}

int main()
{
  LED_init();
  xTaskCreate(red, "red", 1024, NULL, 1, NULL);
  xTaskCreate(blue, "blue", 1024, NULL, 2, NULL);
  xTaskCreate(green, "green", 1024, NULL, 3, NULL);
  vTaskStartScheduler();
  return 0;
}