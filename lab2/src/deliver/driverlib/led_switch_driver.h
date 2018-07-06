#ifndef LED_SWITCH_DRIVER_H
#define LED_SWITCH_DRIVER_H

#include <stdint.h>

#define RCGCGPIO_A_ON  0x01

// GPIO Port A general setting registers.
#define RCGCGPIO       *((volatile unsigned long *) 0x400FE608)
#define GPIO_A_AMSEL   *((volatile unsigned long *) 0x40004528)
#define GPIO_A_PCTL    *((volatile unsigned long *) 0x4000452C)
#define GPIO_A_DIR     *((volatile unsigned long *) 0x40004400)
#define GPIO_A_AFSEL   *((volatile unsigned long *) 0x40004420)
#define GPIO_A_DEN     *((volatile unsigned long *) 0x4000451C)
#define GPIO_A_DATA    *((volatile unsigned long *) 0x400043FC)

// Pins needed for the traffic light system.
#define ON_OFF         0x04     // P2 (start_stop)
#define PASSNGR        0x08     // P3 (passenger)
#define GREEN          0x20     // P5
#define YELLOW         0x40     // P6
#define RED            0x80     // P7

void init(int pin, int input);
unsigned long switch_input(int pin);
void led_ctrl(int pin, int on);
unsigned long int get_PCTL(int pin);

#endif