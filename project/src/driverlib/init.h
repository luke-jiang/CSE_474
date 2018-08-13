#ifndef INIT_H
#define INIT_H

#include <stdint.h>
#include "inc\tm4c123gh6pm.h"

// This function initialize the system clock by configuring the RCC
void initialize_clk() {
  SYSCTL_RCC_R |= (1 << 0); // enable main oscillator
  SYSCTL_RCC_R &= ~(1 << 4); // use the main oscillator as the input source
  SYSCTL_RCC_R &= ~(1 << 5);
  SYSCTL_RCC_R |= (0x3 << 6); // clear XTAL field and configure 16MHz crystal
  SYSCTL_RCC_R |= (1 << 11); // set the bypass to make system clock derived from oscillator source
  SYSCTL_RCC_R &= ~(1 << 22); // do not use system clock divisor
}

// This function describes the behavior for UART#7
void intialize_UART(int ibrd, int fbrd) {
  SYSCTL_RCGCUART_R |= (1 << 7); // enable uart module
  SYSCTL_RCGCGPIO_R |= (1 << 4); // enalbe port E
  GPIO_PORTE_AFSEL_R = (1 << 1) | (1 << 0); // enable alternate function 0-rx 1-tx
  GPIO_PORTE_PCTL_R = (1 << 0) | (1 << 4); // configure PMC1 to tell we are using UART3
  GPIO_PORTE_DEN_R = (1 << 1) | (1 << 0); // enable the digital function
  UART7_CTL_R &= ~(1 << 0); // disable before configuration
  UART7_IBRD_R = ibrd; // BRD = 4M / (153600) = 26.0416667
  UART7_FBRD_R = fbrd; // UARTFBRD = integer(0.0416667 * 64 + 0.5) = 3
  UART7_LCRH_R = (0x3 << 5); // 8 data bits
  UART7_LCRH_R &= ~(1 << 3); // 1 stop bit
  UART7_CC_R = 0x0; // use systme clock
  UART7_CTL_R = (1 << 0) | (1 << 8) | (1 << 9); // enable UART and its reciver and transmitter
}

// This function describes the behavior for M1PWM0(PD0) and M1PWM1(PD1)
void initialize_PWM(int dut1, int dut2) {
    SYSCTL_RCGCPWM_R |= 0x02;
    for (int i = 0; i < 200000; i++);
    SYSCTL_RCGC2_R |= (1 << 3);
    for (int i = 0; i < 200000; i++);
    GPIO_PORTD_AFSEL_R |= (1 << 0);
    GPIO_PORTD_PCTL_R &= ~0x000000FF;
    GPIO_PORTD_PCTL_R |= 0x00000055;
    GPIO_PORTD_DEN_R |= 0x03;
    SYSCTL_RCC_R |= (1 << 20);
    SYSCTL_RCC_R &= ~(1 << 17);
    SYSCTL_RCC_R &= ~(1 << 18);
    SYSCTL_RCC_R &= ~(1 << 19);
    PWM1_0_CTL_R = 0x0;
    PWM1_0_GENA_R = 0x0000008C;
    PWM1_0_LOAD_R = 0x270FF;
    PWM1_0_CMPA_R = dut1;
    //PWM1_0_CMPB_R = dut2;
    PWM1_0_CTL_R |= (1 << 0);
    PWM1_ENABLE_R = 0x03;
}

char read_char() {
    while (UART7_FR_R & (1 << 4)); // wait if receiver is empty
    return (char) UART7_DR_R;
}

void print_char(char c) {
    while (UART7_FR_R & (1 << 5)); // wait if transmitter is full
    UART7_DR_R = c;
}

void print_string(char *str) {
    while (*str) {
        print_char(*str);
        str++;
    }
}

#endif