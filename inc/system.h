#ifndef SYSTEM_H
#define SYSTEM_H

#include "interrupt.h"
#include "uart.h"

#define NUM_MAX_INTERRUPTS (128u)

void ResetIqrHandler(unsigned int iqrNum);
void AddIqrHandler(unsigned int iqrNum, void (*iqrHandler)(void));
#endif