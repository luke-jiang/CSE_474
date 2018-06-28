// CSE 474
// Lab 1 Section A
// Luke Jiang
// 21/06/2018

// This program makes the LED on TM4C123G LaunchPad flash continnuously.

#include "lab1a.h"

int main() {
  // enable port F GPIO
  RCGCGPIO = RCGCGPIO_F_ON;
  // set three LEDs as outputs and enable digital
  GPIO_F_DEN = RGB;
  GPIO_F_DIR = RGB;
  // clear all port F
  GPIO_F_DATA = CLEAR;
  while (1) {
    GPIO_F_DATA = RED;
    for (int i = 0; i < DELAY; i++);
    GPIO_F_DATA = BLUE;
    for (int i = 0; i < DELAY; i++);
    GPIO_F_DATA = GREEN;
    for (int i = 0; i < DELAY; i++);
  }
  return 0;
}
