#ifndef WDT_H
#define WDT_H

#include "soc_AM335x.h"
#include "hw_types.h"
#include "hw_watchdog.h"
#include "clock_module.h"

#define WDT_DISABLE_SEQ1 (0x0000AAAA)
#define WDT_DISABLE_SEQ2 (0x00005555)
#define WDT_ENABLE_SEQ1 (0x0000BBBB)
#define WDT_ENABLE_SEQ2 (0x00004444)

void watchdogTimerEnable();
void watchdogTimerDisable();

#endif