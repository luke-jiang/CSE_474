// CSE 474
// Lab 2
// Luke Jiang
// 06/07/2018

// This header file is the driver for GPTM Timer A1

#ifndef TIMER1_H
#define TIMER1_H

// Registers configuring timer1
#define RCGCTIMER         (*((volatile uint32_t *) 0x400FE604))
#define EN0               (*((volatile uint32_t *) 0xE000E100))
#define T1_CTL            (*((volatile uint32_t *) 0x4003100C))
#define T1_CFG            (*((volatile uint32_t *) 0x40031000))
#define T1_TAMR           (*((volatile uint32_t *) 0x40031004))
#define T1_TAILR          (*((volatile uint32_t *) 0x40031028))
#define T1_RIS            (*((volatile uint32_t *) 0x4003101C))
#define T1_ICR            (*((volatile uint32_t *) 0x40031024))
#define T1_MIMR           (*((volatile uint32_t *) 0x40031018))

// Inputs to T0_TAILR for different speeds
#define SEC1              0x00F42400  // 1 second
#define SEC2              0x01E84800  // 2 seconds
#define SEC5              0x04C4B400  // 5 seconds

#define TAEN_OFF          0x00
#define TAEN_ON           0x01

// Set up timer1 with given speed; allow interrupt if interrupt == 1
void timer1_setup(uint32_t speed, int interrupt);
// Enable timer1 if on == 1, otherwise disable timer1
void timer1_ctrl(int on);
// Return 1 when time is out; else return 0
int timer1_out();
// Clear the timer and begin next period
void timer1_clear();

#endif
