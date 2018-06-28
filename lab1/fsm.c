// CSE 474
// Lab 1 Section B
// Luke Jiang
// 27/06/2018

// This program implements a FSM for the traffic light controller.

#include "driver.h"

// define possible states
typedef enum {
  OFF_STATE,
  GO_STATE,
  WARN_STATE,
  STOP_STATE
} State;

// initialize all LEDs and switches and turn off all LEDs.
void init_all();
// if on == 1, turn on the LED represented by pin. Else, turn off that LED.
void led_ctrl(int pin, int on);

int main() {
  State curr_state = OFF_STATE;
  State next_state;
  init_all();  // initialize switches and leds
  while (1) {
    switch (curr_state) {
      case OFF_STATE:
        delay();
        if (switch_input(ON_OFF)) {
          led_ctrl(GREEN, 1);
          next_state = STOP_STATE;
        } else {
          next_state = curr_state;
        }
        break;
      case GO_STATE:
        delay();
        if (switch_input(ON_OFF)) {
          led_ctrl(GREEN, 0);
          next_state = OFF_STATE;
        } else {
          int i;
          for (i = 0; i < DELAY && !switch_input(PASSNGR); i++);
          if (i < DELAY) {   // passenger
            led_ctrl(GREEN, 0);
            led_ctrl(YELLOW, 1);
            next_state = WARN_STATE;
          } else {
            led_ctrl(GREEN, 0);
            led_ctrl(RED, 1);
            next_state = STOP_STATE;
          }
        }
        break;
      case WARN_STATE:
        if (switch_input(ON_OFF)) {
          led_ctrl(YELLOW, 0);
          next_state = OFF_STATE;
        } else {
          delay();
          led_ctrl(YELLOW, 0);
          led_ctrl(RED, 1);
          next_state = STOP_STATE;
        }
        break;
      case STOP_STATE:
         delay();
         if (switch_input(ON_OFF)) {
           led_ctrl(RED, 0);
           next_state = OFF_STATE;
         } else if (switch_input(PASSNGR)) {
           next_state = STOP_STATE;
         } else {
           led_ctrl(RED, 0);
           led_ctrl(GREEN, 1);
           next_state = GO_STATE;
         }
         break;
    }
    curr_state = next_state;
  }
  return 0;
}

void init_all() {
  switch_init(ON_OFF);
  switch_init(PASSNGR);
  led_init(RED);
  led_off(RED);
  led_init(YELLOW);
  led_off(YELLOW);
  led_init(GREEN);
  led_off(GREEN);
}

void led_ctrl(int pin, int on) {
  if (on) led_on(pin);
  else led_off(pin);
}
