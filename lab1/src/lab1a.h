// CSE 474
// Lab 1 Section A
// Luke Jiang
// 21/06/2018

// This header file includes useful register addresses and data

#ifndef _LAB1A_H_
#define _LAB1A_H_

#include <stdint.h>

#define DELAY   2000000     // time interval of flashing

// Inputs to GPIO_F_DATA for configuring LEDs and switches.
#define RED     0x02        // enable red LED
#define BLUE    0x04        // enable green LED
#define GREEN   0x08        // enable blue LED
#define RGB     0x0E        // enable all three LEDs
#define SWITCH  0x11        // enable switch1 and switch2
#define CLEAR   0x00        // disable all

// Registers configuring GPIO Port F.
#define RCGCGPIO       *((volatile unsigned long *) 0x400FE608)
#define GPIO_F_DIR     *((volatile unsigned long *) 0x40025400)
#define GPIO_F_DEN     *((volatile unsigned long *) 0x4002551C)
#define GPIO_F_DATA    *((volatile unsigned long *) 0x400253FC)

// Input to RCGCGPIO for enabling Port F.
#define RCGCGPIO_F_ON  0x20

// Registers unlocking GPIO PF0.
#define GPIO_LOCK      *((volatile unsigned long *) 0x40025520)
#define GPIO_CR        *((volatile unsigned long *) 0x40025524)
#define GPIO_UR        *((volatile unsigned long *) 0x40025510)

// Input to GPIO_LOCK for unlocking PF0.
#define GPIO_CR_UNLOCK 0x4C4F434B

#endif // lab1.h
