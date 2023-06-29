#ifndef SYSTEM_H
#define SYSTEM_H

typedef void (*InterruptFunction)(void);

void AddIrq(unsigned char irqNum, InterruptFunction function);

void ResetIrq(unsigned char irqNum);

#endif
