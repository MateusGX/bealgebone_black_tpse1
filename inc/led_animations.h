#ifndef LED_ANIMATIONS_H
#define LED_ANIMATIONS_H

typedef enum _state
{
  SEQ1 = 1,
  SEQ2,
  SEQ3,
  SEQ4,
} state;

#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include "interrupt.h"

void anim_1(unsigned int *op, int *flag_timer);
void anim_2(unsigned int *op, int *flag_timer);
void animOff(unsigned int *op, int *flag_timer);
void gpio2IqrHandler(int *btn1_flag, int *btn2_flag);
#endif
