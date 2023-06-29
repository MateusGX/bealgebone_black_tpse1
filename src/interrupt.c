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