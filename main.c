/*****************************************************************************
    Copyright (c) 2013 Gordon Brown. All rights reserved.
******************************************************************************

    File           - Description: Demo code for Adafruit keypad 
    Version Number - Revision: 1 
    Last Updated   - Date: 1/26/13 
    Updated By     - Author: Gordon Brown 
    Authored By    - Gordon Brown
*/

/** @file
	\brief Demo code for Adafruit keypad driver.
*/

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_nvic.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

#include "keypad.h"

#define KEYPAD_ROW_0 GPIO_PIN_0
#define KEYPAD_ROW_1 GPIO_PIN_1
#define KEYPAD_ROW_2 GPIO_PIN_2
#define KEYPAD_ROW_3 GPIO_PIN_3
#define KEYPAD_ROWS (KEYPAD_ROW_0|KEYPAD_ROW_1|KEYPAD_ROW_2|KEYPAD_ROW_3)

#define KEYPAD_COL_LEFT GPIO_PIN_4
#define KEYPAD_COL_MID GPIO_PIN_5
#define KEYPAD_COL_RIGHT GPIO_PIN_6
#define KEYPAD_COLS (KEYPAD_COL_LEFT|KEYPAD_COL_MID|KEYPAD_COL_RIGHT)

#define KEYPAD_ROW_COUNT 4
#define KEYPAD_COL_COUNT 3

#define LED_RED GPIO_PIN_1
#define LED_BLUE GPIO_PIN_2
#define LED_GREEN GPIO_PIN_3

void main(void) {
  // configure system clock to run at 50 MHz
  // use external crystal (16 MHz) and PLL
	SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
	SYSCTL_OSC_MAIN);

	// Enable PORT B GPIO
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	// Enable PORT F GPIO
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	// set LED pins as outputs
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED|LED_BLUE|LED_GREEN);

	initKeypad(GPIO_PORTB_BASE);

	GPIOIntTypeSet(GPIO_PORTB_BASE,KEYPAD_ROWS,GPIO_RISING_EDGE); // set the interrupt type on these pins
	GPIOIntEnable(GPIO_PORTB_BASE,KEYPAD_ROWS); // enable interrupts on these pins
	GPIOIntRegister(GPIO_PORTB_BASE, keyPressISR);
	IntMasterEnable(); // enable interrupts in general

	while(1)
	{
		; // Wait for an interrupt
	}
}
