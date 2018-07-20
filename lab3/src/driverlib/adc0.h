// CSE 474
// Lab 3
// Luke Jiang
// 19/07/2018

// This is the driver file for ADC0 module

#ifndef ADC0_H
#define ADC0_H

#define RCGCADC       *((volatile unsigned long *) 0x400FE638)
#define ADC0_ACTSS    *((volatile unsigned long *) 0x40038000)
#define ADC0_EMUX     *((volatile unsigned long *) 0x40038014)
#define ADC0_SSCTL3   *((volatile unsigned long *) 0x400380A4) 
#define ADC0_SSMUX3   *((volatile unsigned long *) 0x400380A0)
#define ADC0_IM       *((volatile unsigned long *) 0x40038008)
#define ADC0_PSSI     *((volatile unsigned long *) 0x40038028)
#define ADC0_SSFIFO3  *((volatile unsigned long *) 0x400380A8)
#define ADC0_RIS      *((volatile unsigned long *) 0x40038004)
#define ADC0_ISC      *((volatile unsigned long *) 0x4003800C)

#define EN0           *((volatile unsigned long *) 0xE000E100)

void adc0_init() {
    RCGCADC |= 0x01;                    // enable ADC module0 
    for (int i = 0; i < 200000; i++);   // delay to allow the clock to stabilize
    ADC0_ACTSS &= ~0x08;                // disable SS3 sequencer   
    ADC0_EMUX |= (0x05 << 12);          // select timer as software trigger
    ADC0_SSMUX3 = 0x00;                 // select ADC input 0 (PE3)
    ADC0_SSCTL3 |= 0x0E;                // select temp sensor as input, set interrupt
    // setting interrupt
    ADC0_IM |= 0x08;                    // set interrupt mask
    EN0 |= (1 << 17);                   // enable ADC0 at NVIC

    ADC0_ACTSS = 0x08;                  // enable SS3 sequencer
}

void adc0_start() {
    ADC0_PSSI |= 0x08;
}

unsigned long adc0_read() {
    return ADC0_SSFIFO3;
}

void adc0_clear() {
    ADC0_ISC = 0x08;
}


#endif