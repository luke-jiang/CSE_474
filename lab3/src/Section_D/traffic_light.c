// CSE 474
// Lab 3 Section D (5)
// Luke Jiang, Hantao Liu
// 19/07/2018

// Implementation of the traffic light control FSM
// One timer without interrupt.
// Use LCD to implement PASSNGR and STOP/START buttons

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "driverlib/timer0.h"
#include "driverlib/SSD2119.h"
#include "deiverlib/led_lcd.h"


// define possible states
typedef enum {
  OFF_STATE,
  GO_STATE,
  WARN_STATE,
  STOP_STATE
} State;

#define ON_OFF_PRESSED  2
#define PASSNGR_PRESSED 1
#define NO_PRESSED      0

void init_all();
void led_ctrl(int pin, int on);
int delay_go();
int delay_warn();
int delay_stop();
int delay_off();

int main() {
  State curr_state = OFF_STATE;
  State next_state;
  init_all();  // initialize switches and leds
  timer0_setup(SEC1, 0);
  int status;
  while (1) {
    switch (curr_state) {
      case OFF_STATE:
        status = delay_off();
        if (status == ON_OFF_PRESSED) {
          led_ctrl(RED, 1);
          next_state = STOP_STATE;
        } else {
          next_state = curr_state;
        }
        break;
      case GO_STATE:
        status = delay_go();
        if (status == ON_OFF_PRESSED) {
          led_ctrl(GREEN, 0);
        next_state = OFF_STATE;
        } else if (status == PASSNGR_PRESSED){
          led_ctrl(GREEN, 0);
          led_ctrl(YELLOW, 1);
          next_state = WARN_STATE;
        } else {
          led_ctrl(GREEN, 0);
          led_ctrl(RED, 1);
          next_state = STOP_STATE;
        }
        break;
      case WARN_STATE:
        status = delay_warn();
        if (status == ON_OFF_PRESSED) {
          led_ctrl(YELLOW, 0);
          next_state = OFF_STATE;
        } else {
          led_ctrl(YELLOW, 0);
          led_ctrl(RED, 1);
          next_state = STOP_STATE;
        }
        break;
      case STOP_STATE:
        status = delay_stop();
        if (status == ON_OFF_PRESSED) {
          led_ctrl(RED, 0);
          next_state = OFF_STATE;
        } else if (status == PASSNGR_PRESSED) {
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
  init_LED(ON_OFF);
  init_LED(PASSNGR);
  init(RED, 0);
  led_off(RED);
  init(YELLOW, 0);
  led_off(YELLOW);
  init(GREEN, 0);
  led_off(GREEN);
}

void led_ctrl(int pin, int on) {
  if (on) led_on(pin);
  else led_off(pin);
}

int delay_go() {
  timer0_ctrl(1);
  int p_count = 2;
  int s_count = 2;
  for (int i = 5; i > 0; i--) {
    while (!timer0_out());
    if (!switch_input(ON_OFF)) {
      s_count = 2;
    } else {
      s_count--;
      if (!s_count) {
        timer0_ctrl(0);
        return ON_OFF_PRESSED;
      }
    }
    if (!switch_input(PASSNGR)) {
        p_count = 2;
      } else {
        p_count--;
        if (!p_count) {
        timer0_ctrl(0);
        return PASSNGR_PRESSED;
      }
    }
    timer0_clear();
  }
  timer0_ctrl(0);
  return NO_PRESSED;
}

int delay_warn() {
  timer0_ctrl(1);
  int s_count = 2;
  for (int i = 5; i > 0; i--) {
    while (!timer0_out());
    if (!switch_input(ON_OFF)) {
      s_count = 2;
    } else {
      s_count--;
      if (!s_count) {
        timer0_ctrl(0);
        return ON_OFF_PRESSED;
      }
    }
    timer0_clear();
  }
  timer0_ctrl(0);
  return NO_PRESSED;
}

int delay_stop() {
  timer0_ctrl(1);
  int s_count = 2;
  int p_count = 2;
  int ret = NO_PRESSED;
  for (int i = 5; i > 0; i--) {
    while (!timer0_out());
    if (!switch_input(ON_OFF)) {
      s_count = 2;
    } else {
      s_count--;
      if (!s_count) {
        timer0_ctrl(0);
        return ON_OFF_PRESSED;
      }
    }
    if (!switch_input(PASSNGR)) {
      p_count = 2;
    } else {
      p_count--;
      if (!p_count) ret = PASSNGR_PRESSED;
    }
    timer0_clear();
  }
  timer0_ctrl(0);
  return ret;
}

int delay_off() {
  timer0_ctrl(1);
  int s_count = 2;
  for (int i = 5; i > 0; i--) {
    while (!timer0_out());
    if (!switch_input(ON_OFF)) {
      s_count = 2;
    } else {
      s_count--;
    if (!s_count) return ON_OFF_PRESSED;
    }
    timer0_clear();
  }
  timer0_ctrl(0);
  return NO_PRESSED;
}
