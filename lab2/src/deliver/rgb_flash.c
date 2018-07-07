// CSE 474
// Lab 2 Section A1
// Luke Jiang
// 04/07/2018

// Implementing Section A LED Flashing with a timer.

#include "driverlib/timer0.h"
#include "driverlib/port_f.h"

int main() {
  // configure GPIO Port F
  RCGCGPIO = RCGCGPIO_F_ON;
  GPIO_F_DEN = RGB;
  GPIO_F_DIR = RGB;
  GPIO_F_DATA = CLEAR;

  timer0_setup(SEC1, 0);
  timer0_ctrl(1);
  while (1) {
    while (!timer0_out());
    GPIO_F_DATA = RED;
    timer0_clear();
    while (!timer0_out());
    GPIO_F_DATA = BLUE;
    timer0_clear();
    while (!timer0_out());
    GPIO_F_DATA = GREEN;
    timer0_clear();
  }
  return 0;
}
