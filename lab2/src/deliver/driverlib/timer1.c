#include "timer1.h"

void timer1_setup(uint32_t speed, int interrupt) {
  RCGCTIMER |= 0x001;
  T1_CTL = TAEN_OFF;    // disable timer while configuring
  T1_CFG = 0x00;        // select 32-bit timer configuration
  T1_TAMR = 0x02;       // select countdown, periodic mode
  T1_TAILR = speed;     // set speed
  if (interrupt) {
    T1_MIMR = 0x01;     // enable interrupt mask
    EN1 |= (1 << 21);   // enable timer1 at NVIC
    T1_ICR |= 0x01;     // clear timer
  }
}

void timer1_ctrl(int on) {
  if (on) T1_CTL = TAEN_ON;
  else T1_CTL = TAEN_OFF;
}

int timer1_out() {
  return T1_RIS & 0x01;
}

void timer1_clear() {
  T1_ICR |= 0x01;
}