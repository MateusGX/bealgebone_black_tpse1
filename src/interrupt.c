#include "interrupt.h"

#define REG_BIT_MASK (0x1F)

unsigned int getIntcMirClear(unsigned int iqrNum)
{
  unsigned int module = iqrNum >> 5;

  return SOC_AINTC_REGS + INTC_MIR_CLEAR(module);
}

void configureMirClear(unsigned int iqrNum)
{
  unsigned int mir_clear_addr = getIntcMirClear(iqrNum);
  HWREG(mir_clear_addr) |= 1 << (iqrNum & REG_BIT_MASK);
}

void configureIrqGpio(gpioMod mod, ucPinNumber pin)
{
  if (gpioCheckValidPortPin(mod, pin))
  {
    switch (mod)
    {
    case GPIO0:
      return; // GPIO0 doesnt have a clock module register, TRM 8.1.12.1
      break;
    case GPIO1:
      HWREG(SOC_GPIO_1_REGS + GPIO_IRQSTATUS_SET_0) |= 1 << pin;
      HWREG(SOC_GPIO_1_REGS + GPIO_RISINGDETECT) |= 1 << pin;
      HWREG(SOC_GPIO_1_REGS + GPIO_DEBOUNCENABLE) |= 1 << pin;
      HWREG(SOC_GPIO_1_REGS + GPIO_DEBOUNCINGTIME) = DEBOUNCING_TIME;
      break;
    case GPIO2:
      HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS_SET_0) |= 1 << pin;
      HWREG(SOC_GPIO_2_REGS + GPIO_RISINGDETECT) |= 1 << pin;
      HWREG(SOC_GPIO_1_REGS + GPIO_DEBOUNCINGTIME) = DEBOUNCING_TIME;
      break;
    case GPIO3:
      break;
    default:
      break;
    }
  }
}

void clearIrqGpio(gpioMod mod, ucPinNumber pin)
{
  if (gpioCheckValidPortPin(mod, pin))
  {
    switch (mod)
    {
    case GPIO0:
      break;
    case GPIO1:
      HWREG(SOC_GPIO_1_REGS + GPIO_IRQSTATUS_0) = (1 << pin);
      break;
    case GPIO2:
      HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS_0) = (1 << pin);
      break;
    case GPIO3:
      break;
    default:
      break;
    }
  }
}

pinLevel checkIrqGpioPin(gpioMod mod, ucPinNumber pin)
{
  if (gpioCheckValidPortPin(mod, pin))
  {
    switch (mod)
    {
    case GPIO0:
      break;
    case GPIO1:
      return HWREG(SOC_GPIO_1_REGS + GPIO_IRQSTATUS_0) & (1 << pin);
      break;
    case GPIO2:
      return HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS_0) & (1 << pin);
      break;
    case GPIO3:
      break;
    default:
      break;
    }
  }

  return LOW;
}
