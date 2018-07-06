// CSE 474
// Lab 2 Section B2
// Luke Jiang
// 04/07/2018

// When S1 is pressed, the LED turns red; when S2 is pressed,
// the LED blue blinks every 1 second.

#include "driverlib/port_a.h"
#include "driverlib/timer0.h"

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
