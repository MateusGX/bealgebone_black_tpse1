#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "hw_intc.h"
#include "hw_types.h"
#include "gpio.h"
#include "soc_AM335x.h"
#include "system.h"

#define GPIOINT2A 32

void InitIrq(void);
pinLevel checkIrqGpioPin(gpioMod mod, ucPinNumber pin);
unsigned int getIntcMirClear(unsigned int iqrNum);
void configureMirClear(unsigned int iqrNum);
void clearIrqGpio(gpioMod mod, ucPinNumber pin);
void configureIrqGpio(gpioMod mod, ucPinNumber pin);
#endif