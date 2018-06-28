// CSE 474
// Lab 1 Section A
// Luke Jiang
// 21/06/2018

// This program turns red LED on if switch1 is pressed and green LED on
// if switch2 is pressed.

#include "lab1a.h"

int main() {
  // enable port F GPIO
  RCGCGPIO = RCGCGPIO_F_ON;
  // unlock PF0
  GPIO_LOCK = GPIO_CR_UNLOCK;
  GPIO_CR = SWITCH;
  GPIO_UR = SWITCH;
  // set two switches as inputs and three LEDs as outputs and enable digital
  GPIO_F_DEN = RGB | SWITCH;
  GPIO_F_DIR = RGB & ~SWITCH;
  // clear all port F
  GPIO_F_DATA = CLEAR;
  while (1) {
    int switch_1 = GPIO_F_DATA & 0x10;
    int switch_2 = GPIO_F_DATA & 0x01;
    if (!switch_1) {
      GPIO_F_DATA = RED;
    } else if (!switch_2) {
      GPIO_F_DATA = GREEN;
    }
  }
  return 0;
}
