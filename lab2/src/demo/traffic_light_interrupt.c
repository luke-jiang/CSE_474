// CSE 474
// Lab 2 Section B1
// Luke Jiang
// 04/07/2018

// Implementation of the traffic light control FSM
// One timer with interrupt

// driver.h ======================================================
#include <stdint.h>

#define DELAY          2000000
#define RCGCGPIO_A_ON  0x01

// Useful GPIO Port A registers.
#define RCGCGPIO       *((volatile unsigned long *) 0x400FE608)
#define GPIO_A_AMSEL   *((volatile unsigned long *) 0x40004528)
#define GPIO_A_PCTL    *((volatile unsigned long *) 0x4000452C)
#define GPIO_A_DIR     *((volatile unsigned long *) 0x40004400)
#define GPIO_A_AFSEL   *((volatile unsigned long *) 0x40004420)
#define GPIO_A_DEN     *((volatile unsigned long *) 0x4000451C)
#define GPIO_A_DATA    *((volatile unsigned long *) 0x400043FC)

// Pins needed for the traffic light system.
#define ON_OFF         0x04     // P2 (start_stop)
#define PASSNGR        0x08     // P3 (passenger)
#define GREEN          0x20     // P5
#define YELLOW         0x40     // P6
#define RED            0x80     // P7

void init(int pin, int input);
unsigned long switch_input(int pin);
void led_on(int pin);
void led_off(int pin);
unsigned long int get_PCTL(int pin);
void delay();

// timer0.h ======================================================
#define RCGCTIMER         (*((volatile uint32_t *) 0x400FE604))
#define EN0               (*((volatile uint32_t *) 0xE000E100))
#define T0_CTL            (*((volatile uint32_t *) 0x4003000C))
#define T0_CFG            (*((volatile uint32_t *) 0x40030000))
#define T0_TAMR           (*((volatile uint32_t *) 0x40030004))
#define T0_TAILR          (*((volatile uint32_t *) 0x40030028))
#define T0_RIS            (*((volatile uint32_t *) 0x4003001C))
#define T0_ICR            (*((volatile uint32_t *) 0x40030024))
#define T0_MIMR           (*((volatile uint32_t *) 0x40030018))

#define SEC1      0x00F42400
#define SEC2      0x01E84800
#define SEC5      0x04C4B400
#define TAEN_OFF  0x00
#define TAEN_ON   0x01

void timer0_setup(uint32_t speed, int interrupt);
void timer0_ctrl(int on);
int timer0_out();
void timer0_clear();

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

volatile int timer0_flag = 0;

void init_all();
void led_ctrl(int pin, int on);
int delay_go();
int delay_warn();
int delay_stop();
int delay_off();

void Timer_Handler_0A() {
  T0_ICR |= 0x01;
  timer0_flag = 1;
}

int main() {
  State curr_state = OFF_STATE;
  State next_state;
  init_all();  // initialize switches and leds
  timer0_setup(SEC1, 1);
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
  init(ON_OFF, 1);
  init(PASSNGR, 1);
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

int delay_warn() {
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

int delay_stop() {
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

int delay_off() {
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


// timer0.c =================
void timer0_setup(uint32_t speed, int interrupt) {
  RCGCTIMER |= 0x001;

  T0_CTL = TAEN_OFF;    // disable timer while configuring
  T0_CFG = 0x00;        // select 32-bit timer configuration
  T0_TAMR = 0x02;       // select countdown, periodic mode
  T0_TAILR = speed;     // set speed

  if (interrupt) {
    T0_MIMR = 0x01;
    EN0 |= (1 << 19);
    T0_ICR |= 0x01;
  }
}

void timer0_ctrl(int on) {
  if (on) T0_CTL = TAEN_ON;
  else T0_CTL = TAEN_OFF;
}

int timer0_out() {
  return T0_RIS & 0x01;
}

void timer0_clear() {
  T0_ICR |= 0x01;
}

// driver.c ===================
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