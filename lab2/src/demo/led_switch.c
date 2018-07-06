// CSE 474
// Lab 2 Section B2
// Luke Jiang
// 04/07/2018

// When S1 is pressed, the LED turns red; when S2 is pressed,
// the LED blue blinks every 1 second.

#include <stdint.h>

// lab1a.h ==================================================

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

#define GPIO_F_IS      *((volatile unsigned long *) 0x40025404)
#define GPIO_F_IBE     *((volatile unsigned long *) 0x40025408)
#define GPIO_F_IEV     *((volatile unsigned long *) 0x4002540C)
#define GPIO_F_IM      *((volatile unsigned long *) 0x40025410)
#define GPIO_F_ICR     *((volatile unsigned long *) 0x4002541C)

// timer0.h ==================================================
#define RCGCTIMER         (*((volatile uint32_t *) 0x400FE604))
#define EN0               (*((volatile uint32_t *) 0xE000E100))
#define T0_CTL            (*((volatile uint32_t *) 0x4003000C))
#define T0_CFG            (*((volatile uint32_t *) 0x40030000))
#define T0_TAMR           (*((volatile uint32_t *) 0x40030004))
#define T0_TAILR          (*((volatile uint32_t *) 0x40030028))
#define T0_RIS            (*((volatile uint32_t *) 0x4003001C))
#define T0_ICR            (*((volatile uint32_t *) 0x40030024))
#define T0_MIMR           (*((volatile uint32_t *) 0x40030018))

#define SEC1      0x00F42400
#define SEC2      0x01E84800
#define SEC5      0x04C4B400
#define TAEN_OFF  0x00
#define TAEN_ON   0x01

void timer0_setup(uint32_t speed, int interrupt);
void timer0_ctrl(int on);
int timer0_out();
void timer0_clear();
void init_port_f();

volatile int flag = 0;

void Handler_PF(void) {
  GPIO_F_ICR |= 0x11;
  if (!(GPIO_F_DATA & 0x10)) {
      flag = 1;
  } else if (!(GPIO_F_DATA & 0x01)) {
      flag = 2;
  }

}

void Timer_Handler_0A() {
  T0_ICR |= 0x01;
  if (GPIO_F_DATA & 0x04) {
    GPIO_F_DATA = CLEAR;
  } else {
    GPIO_F_DATA = BLUE;
  }
}

int main() {
  init_port_f();
  timer0_setup(SEC1, 1);
  
  while (1) {
    if (flag == 1) {
        flag = 0;
        GPIO_F_DATA = RED;
        timer0_ctrl(0);
    } else if (flag == 2) {
        flag = 0;
        GPIO_F_DATA = BLUE;
        timer0_ctrl(1);
    }
  }
  
  return 0;
}


void init_port_f() {
  RCGCGPIO = RCGCGPIO_F_ON;
  GPIO_LOCK = GPIO_CR_UNLOCK;
  GPIO_CR = SWITCH;
  GPIO_UR = SWITCH;
  GPIO_F_DIR = RGB & ~SWITCH;
  GPIO_F_DEN = RGB | SWITCH;
  GPIO_F_IM |= SWITCH;
  
  GPIO_F_IBE |= SWITCH;
  //GPIO_F_IBE &= ~SWITCH;
  //GPIO_F_IEV |= SWITCH;
  
  GPIO_F_IS = ~SWITCH;
  EN0 |= (1 << 30);
  GPIO_F_DATA = CLEAR;
  GPIO_F_ICR |= SWITCH;
}


void timer0_setup(uint32_t speed, int interrupt) {
  RCGCTIMER |= 0x001;

  T0_CTL = TAEN_OFF;    // disable timer while configuring
  T0_CFG = 0x00;        // select 32-bit timer configuration
  T0_TAMR = 0x02;       // select countdown, periodic mode
  T0_TAILR = speed;     // set speed

  if (interrupt) {
    T0_MIMR = 0x01;
    EN0 |= (1 << 19);
    T0_ICR |= 0x01;
  }
}

void timer0_ctrl(int on) {
  if (on) T0_CTL = TAEN_ON;
  else T0_CTL = TAEN_OFF;
}

int timer0_out() {
  return T0_RIS & 0x01;
}

void timer0_clear() {
  T0_ICR |= 0x01;
}