// CSE 474
// Lab 1 Section B
// Luke Jiang
// 27/06/2018

// This header file includes useful register addresses and data for interfacing
// GPIO Port A and controlling the traffic light system.

#ifndef _DRIVER_H_
#define _DRIVER_H_

#include <stdint.h>

// time delay used in FSM and button debouncing.
#define DELAY          2000000

// Input to RCGCGPIO for enabling Port A.
#define RCGCGPIO_A_ON  0x01

// Useful GPIO Port A registers.
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

// initialize the switch (input == 1) or LED (input == 0) represented by pin.
void init(int pin, int input);
// returns 0x20 if button represented by pin is pressed, else return 0.
unsigned long switch_input(int pin);
// turns on the LED represented by pin.
void led_on(int pin);
// turns off the LED represented by pin.
void led_off(int pin);
// helper function that takes a pin as input, calculates and returns the
// corresponding value to be written to GPIOPCTL.
unsigned long int get_PCTL(int pin);
// time delay for about 2 seconds.
void delay();

#endif
