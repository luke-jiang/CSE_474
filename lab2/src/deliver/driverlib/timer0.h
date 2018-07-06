#ifndef TIMER0_H
#define TIMER0_H

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

#endif