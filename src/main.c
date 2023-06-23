#include "bbb_regs.h"
#include "soc_AM335x.h"
#include "hw_intc.h"
#include "hw_types.h"
#include "interrupt.h"
#include "timer.h"
#include "gpio.h"
#include "wdt.h"
#include "uart.h"
#include "led_animations.h"

int flag_timer = false;
int flagBtn1 = 1;
int flagBtn2 = 1;

void btn1Handler(void)
{
	uartPutString(UART0, "B1", 2);
}
void btn2Handler(void)
{
	uartPutString(UART0, "B2", 2);
}

void timerIrqHandler(void)
{

	/* Clear the status of the interrupt flags */
	HWREG(BBB_DMTIMER_IRQSTATUS) = 0x2;

	flag_timer = true;

	/* Stop the DMTimer */
	DMTimerDisable(SOC_DMTIMER_7_REGS);

	// Pisca o led
	//((flag_timer++ & 0x1) ? ledOn() : ledOff());
}

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
	gpioPinMuxSetup(GPIO2, 1); // led
	gpioPinMuxSetup(GPIO2, 3); // TIMER 7 -btn
	gpioPinMuxSetup(GPIO2, 4); // TIMER 6 -btn

	gpioSetDirection(GPIO2, 1, OUTPUT);
	gpioSetDirection(GPIO2, 3, INPUT);
	gpioSetDirection(GPIO2, 4, INPUT);

	// IRQ
	configureMirClear(GPIOINT2A);
	configureIrqGpio(GPIO2, 3);
	configureIrqGpio(GPIO2, 4);
}

void ISR_Handler(void)
{
	unsigned int irq_number = HWREG(BBB_INTC_SIR_IRQ) & 0x7f;
	if (irq_number == TINT7)
	{
		uartPutString(UART0, "IRQ -> TIMER\r\n", 14);
		timerIrqHandler();
	}
	else if (irq_number == GPIOINT2A)
	{
		gpio2IqrHandler(&flagBtn1, &flagBtn2);
	}
	/* Reconhece a IRQ */
	HWREG(BBB_INTC_CONTROL) = 0x1;
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

	uartPutString(UART0, "SISTEMA INICIADO\r\n", 18);

	while (true)
	{
		if (flagBtn2 == HIGH)
		{
			animOff(&op3, &flag_timer);
			continue;
		}

		if (flagBtn1 == HIGH)
			anim_1(&op1, &flag_timer);
		else
			anim_2(&op2, &flag_timer);
	}

	return (0);
}