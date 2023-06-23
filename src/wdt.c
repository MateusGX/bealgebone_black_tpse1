#include "wdt.h"

#define PEND_NONE 0x0

void waitWdtWrite();

void watchdogTimerEnable()
{
  HWREG(SOC_WDT_1_REGS + WDT_WSPR) = WDT_ENABLE_SEQ1;

  waitWdtWrite();

  HWREG(SOC_WDT_1_REGS + WDT_WSPR) = WDT_ENABLE_SEQ2;

  waitWdtWrite();
}

void watchdogTimerDisable()
{
  HWREG(SOC_WDT_1_REGS + WDT_WSPR) = WDT_DISABLE_SEQ1;

  waitWdtWrite();

  HWREG(SOC_WDT_1_REGS + WDT_WSPR) = WDT_DISABLE_SEQ2;

  waitWdtWrite();
}

void waitWdtWrite()
{
  while ((HWREG(SOC_WDT_1_REGS + WDT_WWPS) & WDT_WWPS_W_PEND_WSPR) != PEND_NONE)
    ;
}