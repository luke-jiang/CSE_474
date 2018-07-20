// CSE 474
// Lab 3 Section D (2)
// Luke Jiang, Hantao Liu
// 19/07/2018

// This program implements the thermometer functionality described in Section D (1).
// Press the left button to increase clock frequency and the right button to de
// -crease the frequency. Display the temperature on th LCD board.

#include <stdio.h>
#include "driverlib/port_f.h"
#include "driverlib/port_e.h"
#include "driverlib/timer0.h"
#include "driverlib/adc0.h"
#include "driverlib/sys_clock.h"
#include "driverlib/SSD2119.h"
#include "./inc/tm4c123gh6pm.h"


void port_f_init();
void port_e_init();

void ADC0_Handler() {
    double adc_code = (double) adc0_read();
    double temp = 147.5 - (247.5) * adc_code / 4096.0;
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
    char tempstr[3];
    sprintf(tempstr, "%d", (int) temp);
    LCD_PrintString(tempstr);
    LCD_PrintString("\n");
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
    LCD_Init();
    LCD_GPIOInit();
    while (1) {
        if (!(GPIO_F_DATA & 0x01)) {
            sysclock_setup(MHZ_4);
            timer0_setup(SEC1_4, 1);
            timer0_ctrl(1);
        } else if (!(GPIO_F_DATA & 0x10)) {
            sysclock_setup(MHZ_80);
            timer0_setup(SEC1_80, 1);
            timer0_ctrl(1);
        }
    }
    return 1;
}

void port_f_init() {
    RCGCGPIO |= RCGCGPIO_F_ON;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
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