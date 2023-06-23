#include "interrupt.h"

#define REG_BIT_MASK (0x1F)

unsigned int getIntcMirClear(unsigned int iqrNum)
{
  unsigned int module = iqrNum >> 5;

  switch (module)
  {
  case 0:
  case 1:
  case 2:
  case 3:
    return SOC_AINTC_REGS + INTC_MIR_CLEAR(module);
  default:
    return 0x0;
  }
}

void configureMirClear(unsigned int iqrNum)
{
  unsigned int mir_clear_addr = getIntcMirClear(iqrNum);
  if (mir_clear_addr == 0x0)
    return;

  HWREG(mir_clear_addr) = iqrNum & REG_BIT_MASK;
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
      // HWREG(SOC_GPIO_1_REGS + GPIO_DEBOUNCENABLE) |= 1 << pin;
      break;
    case GPIO2:
      HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS_SET_0) |= 1 << pin;
      HWREG(SOC_GPIO_2_REGS + GPIO_RISINGDETECT) |= 1 << pin;
      // HWREG(SOC_GPIO_2_REGS + GPIO_DEBOUNCENABLE) |= 1 << pin;
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

void InitIrq(void)
{
  /* Reset the ARM interrupt controller */
  HWREG(SOC_AINTC_REGS + INTC_SYSCONFIG) |= (1U << 0);
  HWREG(SOC_AINTC_REGS + INTC_IDLE) |= (1U << 1) | (1U << 0);

  for (int i = 0; i < NUM_MAX_INTERRUPTS; i++)
  {
    ResetIqrHandler(i);
  }
}