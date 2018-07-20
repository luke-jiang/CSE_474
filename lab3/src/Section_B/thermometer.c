// CSE 474
// Lab 3 Section B
// Luke Jiang, Hantao Liu
// 19/07/2018

// This program implements the thermometer functionality described in Section B.
// Press the left button to increase clock frequency and the right button to de
// -crease the frequency. The temperature is displayed in PUTTY.

#include <stdio.h>
#include "driverlib/port_f.h"
#include "driverlib/port_e.h"
#include "driverlib/timer0.h"
#include "driverlib/adc0.h"
#include "driverlib/sys_clock.h"
#include "driverlib/uart0.h"

void port_f_init();
void port_e_init();
void temp_display(int temp);

void ADC0_Handler() {
    double adc_code = (double) adc0_read();
    double temp = 147.5 - (247.5) * adc_code / 4096.0;
    temp_display(temp);
    char tempstr[3];
    sprintf(tempstr, "%d", (int) temp);
    print_string(tempstr);
    print_string("\n");
    adc0_clear();
}

void Timer_Handler_0A() {
    timer0_clear();
    adc0_start();
}

int main() {
    port_f_init();
    port_e_init();
    adc0_init();
    while (1) {
        if (!(GPIO_F_DATA & 0x10)) {        // PF4 -> 80MHz
            sysclock_setup(MHZ_80);
            uart0_setup(BRD_80, DIVFRAC_80);
            timer0_setup(SEC1_80, 1);
            timer0_ctrl(1);
        } else if (!(GPIO_F_DATA & 0x01)) {  // PF0 -> 4MHz
            sysclock_setup(MHZ_4);
            uart0_setup(BRD_4, DIVFRAC_4);
            timer0_setup(SEC1_4, 1);
            timer0_ctrl(1);    
        }
        
    }
}

void port_f_init() {
    RCGCGPIO |= RCGCGPIO_F_ON;
    GPIO_LOCK = GPIO_CR_UNLOCK;
    GPIO_CR = SWITCH;
    GPIO_UR = SWITCH;
    GPIO_F_DIR = RGB & ~SWITCH;
    GPIO_F_DEN = RGB | SWITCH;
    GPIO_F_DATA = CLEAR;
}

void port_e_init() {
    RCGCGPIO |= RCGCGPIO_E_ON;
    GPIO_E_DIR &= ~PE3;
    GPIO_E_AFSEL |= PE3;
    GPIO_E_DEN &= ~PE3;
    GPIO_E_AMSEL |= PE3;
}

void temp_display(int temp) {
    if (temp < 17.0) {
        GPIO_F_DATA = RED;
    } else if (temp < 19.0) {
        GPIO_F_DATA = BLUE;
    } else if (temp < 21.0) {
        GPIO_F_DATA = VIOLET;
    } else if (temp < 23.0) {
        GPIO_F_DATA = GREEN;
    } else if (temp < 25.0) {
        GPIO_F_DATA = YELLOW;
    } else if (temp < 27.0) {
        GPIO_F_DATA = LBLUE;
    } else {
        GPIO_F_DATA = WHITE;
    }
}