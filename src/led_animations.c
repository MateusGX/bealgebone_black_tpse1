#include "led_animations.h"

void ledOn(gpioMod mod, ucPinNumber pin)
{
  gpioSetPinValue(mod, pin, HIGH);
}

void ledOff(gpioMod mod, ucPinNumber pin)
{
  gpioSetPinValue(mod, pin, LOW);
}

void anim_1(unsigned int *op)
{
  switch (*op)
  {
  case SEQ1:
    // 0 1 1 0
    ledOff(GPIO1, 21);
    ledOff(GPIO1, 24);
    ledOn(GPIO1, 22);
    ledOn(GPIO1, 23);
    DelayIrq(1000);
    *op = SEQ2;
    break;
  case SEQ2:
    // 0 0 1 1
    ledOff(GPIO1, 21);
    ledOff(GPIO1, 22);
    ledOn(GPIO1, 23);
    ledOn(GPIO1, 24);
    DelayIrq(1000);
    *op = SEQ3;
    break;
  case SEQ3:
    // 1 0 0 1
    ledOff(GPIO1, 22);
    ledOff(GPIO1, 23);
    ledOn(GPIO1, 21);
    ledOn(GPIO1, 24);
    DelayIrq(1000);
    *op = SEQ4;
    break;
  case SEQ4:
    // 1 1 0 0
    ledOff(GPIO1, 21);
    ledOn(GPIO1, 22);
    ledOff(GPIO1, 24);
    ledOn(GPIO1, 21);
    DelayIrq(1000);
    ledOn(GPIO2, 1);
    DelayIrq(1000);
    ledOff(GPIO2, 1);
    *op = SEQ1;
    break;
  default:
    break;
  }
}

void anim_2(unsigned int *op)
{
  switch (*op)
  {
  case SEQ1:
    ledOn(GPIO1, 21);
    ledOn(GPIO1, 22);
    ledOn(GPIO1, 23);
    ledOn(GPIO1, 24);
    ledOn(GPIO2, 1);
    DelayIrq(1000);
    *op = SEQ2;
    break;
  case SEQ2:
    ledOff(GPIO1, 21);
    ledOff(GPIO1, 22);
    ledOff(GPIO1, 23);
    ledOff(GPIO1, 24);
    ledOff(GPIO2, 1);
    DelayIrq(1000);
    *op = SEQ1;
    break;
  default:
    break;
  }
}

void animOff(unsigned int *op)
{
  ledOff(GPIO1, 21);
  ledOff(GPIO1, 22);
  ledOff(GPIO1, 23);
  ledOff(GPIO1, 24);
  switch (*op)
  {
  case SEQ1:
    ledOn(GPIO2, 1);
    DelayIrq(1000);
    *op = SEQ2;
    break;
  case SEQ2:
    ledOff(GPIO2, 1);
    DelayIrq(1000);
    *op = SEQ1;
    break;
  default:
    break;
  }
}