// CSE 474
// Lab 1 Section B
// Luke Jiang
// 27/06/2018

// This program implements functionalities declared in driver.h

#include "driver.h"

void init(int pin, int input) {
  RCGCGPIO |= RCGCGPIO_A_ON;        // activate clock for Port A
  GPIO_A_AMSEL &= ~pin;             // disable analog on pin
  GPIO_A_PCTL &= ~get_PCTL(pin);    // PCTL GPIO on pin
  if (input) GPIO_A_DIR &= ~pin;    // direction pin input
  else GPIO_A_DIR |= pin;           // direction pin output
  GPIO_A_AFSEL &= ~pin;             // pin regular port function
  GPIO_A_DEN |= pin;                // enable pin digital port
}

unsigned long switch_input(int pin) {
  delay();    // debouncing
  return GPIO_A_DATA & pin;
}

void led_on(int pin) {
  GPIO_A_DATA |= pin;
}

void led_off(int pin) {
  GPIO_A_DATA &= ~pin;
}

unsigned long int get_PCTL(int pin) {
  unsigned long int res = 0x0000000F;
  while (pin > 1) {
    res = res << 4;
    pin = pin >> 1;
  }
  return res;
}

void delay() {
  for (int i = 0; i < DELAY; i++);
}
