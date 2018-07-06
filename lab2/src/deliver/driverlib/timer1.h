#ifndef TIMER1_H
#define TIMER1_H

#define RCGCTIMER         (*((volatile uint32_t *) 0x400FE604))
#define EN0               (*((volatile uint32_t *) 0xE000E100))
#define T1_CTL            (*((volatile uint32_t *) 0x4003100C))
#define T1_CFG            (*((volatile uint32_t *) 0x40031000))
#define T1_TAMR           (*((volatile uint32_t *) 0x40031004))
#define T1_TAILR          (*((volatile uint32_t *) 0x40031028))
#define T1_RIS            (*((volatile uint32_t *) 0x4003101C))
#define T1_ICR            (*((volatile uint32_t *) 0x40031024))
#define T1_MIMR           (*((volatile uint32_t *) 0x40031018))

#define SEC1      0x00F42400
#define SEC2      0x01E84800
#define SEC5      0x04C4B400
#define TAEN_OFF  0x00
#define TAEN_ON   0x01

void timer1_setup(uint32_t speed, int interrupt);
void timer1_ctrl(int on);
int timer1_out();
void timer1_clear();

#endif
