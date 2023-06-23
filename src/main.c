/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  12/02/2017 20:05:55
 *       Revision:  none
 *       Compiler:  arm-none-eabi-gcc
 *
 *         Author:  Francisco Helder (FHC), helderhdw@gmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

#include "uart.h"
#include "gpio.h"
#include "wdt.h"
#include "timer.h"
#include "interrupt.h"
#include "system.h"

/*****************************************************************************
**                INTERNAL MACRO DEFINITIONS
*****************************************************************************/
typedef enum _state
{
	SEQ1 = 1,
	SEQ2,
	SEQ3,
	SEQ4,
} state;

/*****************************************************************************
**                INTERNAL FUNCTION PROTOTYPES
*****************************************************************************/
void anim_2(unsigned int *op);
void anim_1(unsigned int *op);
void ledON(gpioMod, ucPinNumber);
void ledOFF(gpioMod, ucPinNumber);
void animOff(unsigned int *op);

void btn1Handler(void);
void btn2Handler(void);
void gpio2IqrHandler(void);

/*****************************************************************************
**                					INTERNAL VARIABLES
*****************************************************************************/
unsigned int flagBtn1 = 1;
unsigned int flagBtn2 = 1;

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:
 * =====================================================================================
 */

void gpioSetup()
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

int main(void)
{
	unsigned int op1 = SEQ1;
	unsigned int op2 = SEQ1;
	unsigned int op3 = SEQ1;

	watchdogTimerDisable();
	InitIrq();
	gpioSetup();
	uartInitModule(UART0, 115200, STOP1, PARITY_NONE, FLOW_OFF);
	DMTimerSetUp();
	uartPutC(UART0, 'R');
	AddIqrHandler(GPIOINT2A, gpio2IqrHandler);
	uartPutC(UART0, 'S');

	while (true)
	{
		if (flagBtn2 == HIGH)
		{
			animOff(&op3);
			continue;
		}

		if (flagBtn1 == HIGH)
			anim_1(&op1);
		else
			anim_2(&op2);

		Delay(500);
	}

	return (0);
} /* ----------  end of function main  ---------- */

void anim_1(unsigned int *op)
{
	switch (*op)
	{
	case SEQ1:
		// 0 1 1 0
		ledOFF(GPIO1, 21);
		ledOFF(GPIO1, 24);
		ledON(GPIO1, 22);
		ledON(GPIO1, 23);
		Delay(1000);
		*op = SEQ2;
		break;
	case SEQ2:
		// 0 0 1 1
		ledOFF(GPIO1, 21);
		ledOFF(GPIO1, 22);
		ledON(GPIO1, 23);
		ledON(GPIO1, 24);
		Delay(1000);
		*op = SEQ3;
		break;
	case SEQ3:
		// 1 0 0 1
		ledOFF(GPIO1, 22);
		ledOFF(GPIO1, 23);
		ledON(GPIO1, 21);
		ledON(GPIO1, 24);
		Delay(1000);
		*op = SEQ4;
		break;
	case SEQ4:
		// 1 1 0 0
		ledOFF(GPIO1, 21);
		ledON(GPIO1, 22);
		ledOFF(GPIO1, 24);
		ledON(GPIO1, 21);
		Delay(1000);
		ledON(GPIO2, 1);
		Delay(1000);
		ledOFF(GPIO2, 1);
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
		ledON(GPIO1, 21);
		ledON(GPIO1, 22);
		ledON(GPIO1, 23);
		ledON(GPIO1, 24);
		ledON(GPIO2, 1);
		Delay(1000);
		*op = SEQ2;
		break;
	case SEQ2:
		ledOFF(GPIO1, 21);
		ledOFF(GPIO1, 22);
		ledOFF(GPIO1, 23);
		ledOFF(GPIO1, 24);
		ledOFF(GPIO2, 1);
		Delay(1000);
		*op = SEQ1;
		break;
	default:
		break;
	}
}

void animOff(unsigned int *op)
{
	switch (*op)
	{
	case SEQ1:
		ledON(GPIO2, 1);
		Delay(1000);
		*op = SEQ2;
		break;
	case SEQ2:
		ledOFF(GPIO2, 1);
		Delay(1000);
		*op = SEQ1;
		break;
	default:
		break;
	}
}

void gpio2IqrHandler(void)
{
	if (checkIrqGpioPin(GPIO2, 3))
	{
		btn1Handler();
		clearIrqGpio(GPIO2, 3);
	}
	else if (checkIrqGpioPin(GPIO2, 4))
	{
		btn2Handler();
		clearIrqGpio(GPIO2, 4);
	}
}

void btn1Handler(void)
{
	uartPutString(UART0, "B1", 2);
	flagBtn1 = !flagBtn1;
}
void btn2Handler(void)
{
	uartPutString(UART0, "B2", 2);
	flagBtn2 = !flagBtn2;

	if (flagBtn2 == HIGH)
	{
		ledOFF(GPIO1, 21);
		ledOFF(GPIO1, 22);
		ledOFF(GPIO1, 23);
		ledOFF(GPIO1, 24);
	}
}

void ledON(gpioMod mod, ucPinNumber pin)
{
	gpioSetPinValue(mod, pin, HIGH);
}

void ledOFF(gpioMod mod, ucPinNumber pin)
{
	gpioSetPinValue(mod, pin, LOW);
}