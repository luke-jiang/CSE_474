// CSE 474
// Lab 2 Section B1
// Luke Jiang
// 04/07/2018

// Implementation of the traffic light control FSM with one timer (timer0)
// and interrupt enabled.

#include "driverlib/led_switch_driver.h"
#include "driverlib/timer0.h"

// define possible states
typedef enum {
  OFF_STATE,
  GO_STATE,
  WARN_STATE,
  STOP_STATE
} State;

// define possible events
typedef enum {
  NO_PRESSED,
  PASSNGR_PRESSED,
  NO_PRESSED
} Event;

// Interrupt flag for timer0
volatile int timer0_flag = 0;

// initialize switches ans LEDs
void init_all();
Event delay_go();
Event delay_warn();
Event delay_stop();
Event delay_off();

void Timer_Handler_0A() {
  T0_ICR |= 0x01;
  timer0_flag = 1;
}

int main() {
  State curr_state = OFF_STATE;
  State next_state;
  init_all();
  timer0_setup(SEC1, 1);
  Event status;
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
  init(ON_OFF, 1);
  init(PASSNGR, 1);
  init(RED, 0);
  led_off(RED);
  init(YELLOW, 0);
  led_off(YELLOW);
  init(GREEN, 0);
  led_off(GREEN);
}

Event delay_go() {
  timer0_ctrl(1);
  int p_count = 2;
  int s_count = 2;
  for (int i = 5; i > 0; i--) {
    while (!timer0_flag);
    timer0_flag = 0;
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

Event delay_warn() {
  timer0_ctrl(1);
  int s_count = 2;
  for (int i = 5; i > 0; i--) {
    while (!timer0_flag);
    timer0_flag = 0;
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

Event delay_stop() {
  timer0_ctrl(1);
  int s_count = 2;
  int p_count = 2;
  int ret = NO_PRESSED;
  for (int i = 5; i > 0; i--) {
    while (!timer0_flag);
    timer0_flag = 0;
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

Event delay_off() {
  timer0_ctrl(1);
  int s_count = 2;
  for (int i = 5; i > 0; i--) {
    while (!timer0_flag);
    timer0_flag = 0;
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
