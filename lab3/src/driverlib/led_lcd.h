// CSE 474
// Lab 2 Section D (5)
// Luke Jiang
// 04/07/2018

// This header file provides interfacing with LCD and LED.

#ifndef LED_LCD_H
#define LED_LCD_H

#include "driverlib/tm4c123gh6pm.h"
#include "driverlib/SSD2119.h"

// Pins needed for the traffic light system.
#define ON_OFF         0x04     // P2 (start_stop)
#define PASSNGR        0x08     // P3 (passenger)
#define GREEN          0x20     // P5
#define YELLOW         0x40     // P6
#define RED            0x80     // P7


// Initialize A GPIO Port C pin as LED input
void init_LED(int pin) {
    SYSCTL_RCGC2_R |= 0x04;               // activate clock for Port C
    GPIO_PORTC_AMSEL &= ~pin;             // disable analog on pin
    GPIO_PORTC_PCTL_R &= ~get_PCTL(pin);  // PCTL GPIO on pin
    GPIO_PORTC_DIR_R |= pin;              // direction pin output
    GPIO_PORTC_AFSEL_R &= ~pin;           // pin regular port function
    GPIO_PORTC_DEN_R |= pin;              // enable pin digital port
}

// Read touch input from LCD. A valid PASSNGR read is when yPos > 160.
// A valid ON/OFF read is when yPos > 60.
unsigned long switch_input(int pin) {
    Touch_readX();
    Touch_readY();
    long res = Touch_GetCoords();
    long yPos = res & 0xFF;
    long xPos = res >> 16;
    LCD_PrintInteger(yPos);
    LCD_PrintChar(' ');
    LCD_PrintInteger(xPos);
    LCD_PrintChar(' ');
    LCD_PrintChar('\n');
    if (pin == PASSNGR) {
        return yPos > 160;
    } else {
        return yPos > 60;
    }
}

// Turn on the LED.
void led_on(int pin) {
    GPIO_PORTC_DATA_R |= pin;
}

// Turn off the LED.
void led_off(int pin) {
    GPIO_PORTC_DATA_R  &= ~pin;
}

// Helper function that takes a pin as input, calculates and returns the
// corresponding value to be written to GPIOPCTL.
unsigned long int get_PCTL(int pin) {
    unsigned long int res = 0x0000000F;
    while (pin > 1) {
        res = res << 4;
        pin = pin >> 1;
    }
    return res;
}

#endif
