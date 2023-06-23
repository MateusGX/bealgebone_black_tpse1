#include "system.h"

void (*fnRAMVectors[NUM_MAX_INTERRUPTS])(void);

static void IntDefaultHandler(void)
{
  /* Go Back. Nothing to be done */
  ;
}

void AddIqrHandler(unsigned int iqrNum, void (*iqrHandler)(void))
{
  if (iqrNum >= 0 && iqrNum < 128)
  {
    fnRAMVectors[iqrNum] = iqrHandler;
  }
}

void ResetIqrHandler(unsigned int iqrNum)
{
  if (iqrNum >= 0 && iqrNum < 128)
  {
    fnRAMVectors[iqrNum] = IntDefaultHandler;
  }
}

void GlobalIqrHandler(void)
{
  unsigned int irq_number = HWREG(INTC_SIR_IRQ) & 0x7f;
  uartPutString(UART0, "I\r\n", 3);
  void (*irq_handler)() = fnRAMVectors[irq_number];
  (*irq_handler)();

  HWREG(SOC_AINTC_REGS + INTC_CONTROL) = 0x1;
}