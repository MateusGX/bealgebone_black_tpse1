#include "bbb_regs.h"
#include "soc_AM335x.h"
#include "hw_intc.h"
#include "hw_types.h"
#include "interrupt.h"
#include "timer.h"
#include "gpio.h"
#include "wdt.h"
#include "uart.h"
#include "system.h"
#include "led_animations.h"

static int flagGpio2Btn1 = HIGH;
static int flagGpio2Btn2 = HIGH;

void setupGpio()
{
	gpioInitModule(GPIO1);

	// USR PINS
	gpioPinMuxSetup(GPIO1, 21);
	gpioPinMuxSetup(GPIO1, 22);
	gpioPinMuxSetup(GPIO1, 23);
	gpioPinMuxSetup(GPIO1, 24);

	gpioSetDirection(GPIO1, 21, OUTPUT);
	gpioSetDirection(GPIO1, 22, OUTPUT);
	gpioSetDirection(GPIO1, 23, OUTPUT);
	gpioSetDirection(GPIO1, 24, OUTPUT);

	gpioInitModule(GPIO2);
	gpioPinMuxSetup(GPIO2, 1);	// led
	gpioPinMuxSetup(GPIO1, 12); // led irq
	gpioPinMuxSetup(GPIO2, 3);	// TIMER 7 -btn
	gpioPinMuxSetup(GPIO2, 4);	// TIMER 6 -btn

	gpioSetDirection(GPIO2, 1, OUTPUT);
	gpioSetDirection(GPIO1, 12, OUTPUT);
	gpioSetDirection(GPIO2, 3, INPUT);
	gpioSetDirection(GPIO2, 4, INPUT);

	// IRQ
	configureMirClear(GPIOINT2A);
	configureIrqGpio(GPIO2, 3);
	configureIrqGpio(GPIO2, 4);
}

void gpio2Handle(void)
{
	gpio2IqrHandler(&flagGpio2Btn1, &flagGpio2Btn2);
}

int main(void)
{
	unsigned int op1 = SEQ1;
	unsigned int op2 = SEQ1;
	unsigned int op3 = SEQ1;

	watchdogTimerDisable();
	uartInitModule(UART0, 115200, STOP1, PARITY_NONE, FLOW_OFF);
	DMTimerSetUp();
	setupGpio();

	// IRQs
	AddIrq(TINT7, timerIrqHandler);
	AddIrq(GPIOINT2A, gpio2Handle);

	uartPutString(UART0, "SISTEMA INICIADO\r\n", 18);

	while (true)
	{
		if (flagGpio2Btn2 == HIGH)
		{
			animOff(&op3);
			continue;
		}

		if (flagGpio2Btn1 == HIGH)
			anim_1(&op1);
		else
			anim_2(&op2);
	}

	return (0);
}
