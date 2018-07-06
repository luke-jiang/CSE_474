// CSE 474
// Lab 2
// Luke Jiang
// 06/07/2018

// This header file is the driver for two pushbuttons and three LEDs
// in the traffic light control circuit.

#ifndef LED_SWITCH_DRIVER_H
#define LED_SWITCH_DRIVER_H

#include <stdint.h>

// Input to RCGCGPIO for enabling Port A.
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
#define ON_OFF         0x04     // PA2 (start_stop)
#define PASSNGR        0x08     // PA3 (passenger)
#define GREEN          0x20     // PA5
#define YELLOW         0x40     // PA6
#define RED            0x80     // PA7

// initialize the switch (input == 1) or LED (input == 0) represented by pin.
void init(int pin, int input);
// returns 0x20 if button represented by pin is pressed, else return 0.
unsigned long switch_input(int pin);
// turn on the LED represented by pin if on == 1, else turn off
void led_ctrl(int pin, int on);
// helper function that takes a pin as input, calculates and returns the
// corresponding value to be written to GPIOPCTL.
unsigned long int get_PCTL(int pin);

#endif