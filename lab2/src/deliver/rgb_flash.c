// CSE 474
// Lab 2 Section A1
// Luke Jiang
// 04/07/2018

// Implementing Section A LED Flashing with a timer.

#include "driverlib/timer0.h"
#include "driverlib/port_a.h"

int main() {
  // enable port F GPIO
  RCGCGPIO = RCGCGPIO_F_ON;
  // set three LEDs as outputs and enable digital
  GPIO_F_DEN = RGB;
  GPIO_F_DIR = RGB;
  // clear all port F
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
