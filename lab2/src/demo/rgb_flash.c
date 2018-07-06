// CSE 474
// Lab 2 Section A1
// Luke Jiang
// 04/07/2018

// Implementing Section A LED Flashing.

#include <stdint.h>

// timer.h =====================================================
#define RCGCTIMER         (*((volatile uint32_t *) 0x400FE604))
#define TMR0_CTL          (*((volatile uint32_t *) 0x4003000C))
#define TMR0_CFG          (*((volatile uint32_t *) 0x40030000))
#define TMR0_TAMR         (*((volatile uint32_t *) 0x40030004))
#define TMR0_TAILR        (*((volatile uint32_t *) 0x40030028))
#define TMR0_RIS          (*((volatile uint32_t *) 0x4003001C))
#define TMR0_ICR          (*((volatile uint32_t *) 0x40030024))

#define SEC1      0x00F42400
#define TAEN_OFF  0x00
#define TAEN_ON   0x01

// lab1a.h ======================================================
#define DELAY   2000000     // time interval of flashing

// Inputs to GPIO_F_DATA for configuring LEDs and switches.
#define RED     0x02        // enable red LED
#define BLUE    0x04        // enable blue LED
#define GREEN   0x08        // enable green LED
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

void timer0_setup();
void timer0_ctrl();
int time_out();
void clear();
void timer(int n);

int main() {
  // enable port F GPIO
  RCGCGPIO = RCGCGPIO_F_ON;
  // set three LEDs as outputs and enable digital
  GPIO_F_DEN = RGB;
  GPIO_F_DIR = RGB;
  // clear all port F
  GPIO_F_DATA = CLEAR;

  timer0_setup();
  timer0_ctrl(1);
  while (1) {
    while (!time_out());
    GPIO_F_DATA = RED;
    clear();
    while (!time_out());
    GPIO_F_DATA = BLUE;
    clear();
    while (!time_out());
    GPIO_F_DATA = GREEN;
    clear();
  }
  return 0;
}

void timer0_setup() {
  RCGCTIMER = 0x01;       // select timer0 to use
  TMR0_CTL = TAEN_OFF;    // disable timer while configuring
  TMR0_CFG = 0x00;        // select 32-bit timer configuration
  TMR0_TAMR = 0x02;       // select countdown, periodic mode
  TMR0_TAILR = SEC1;      // set speed = 16,000,000
}

void timer0_ctrl(int on) {
  if (on) TMR0_CTL = TAEN_ON;
  else TMR0_CTL = TAEN_OFF;
}

int time_out() {
  return TMR0_RIS & 0x01;
}

void clear() {
  TMR0_ICR |= 0x01;
}

void timer(int n) {
  timer0_ctrl(1);
  for (int i = 0; i < n; i++) {
    while (!time_out());
    clear();
  }
  timer0_ctrl(0);
}
