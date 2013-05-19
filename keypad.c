/*****************************************************************************
	Copyright (c) 2013 Gordon Brown. All rights reserved.
******************************************************************************

	File           - Description: Keypad library for Stellaris implementation. 
	Version Number - Revision: 1 
	Last Updated   - Date: 1/26/2013 
	Updated By     - Author: Gordon Brown 
	Authored By    - Gordon Brown
*/

/** @file
	\brief Implementation for Adafruit keypad library.
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

#define KEYPAD_ROW_0 GPIO_PIN_0 ///< The row beginning with 1.
#define KEYPAD_ROW_1 GPIO_PIN_1 ///< The row beginning with 4.
#define KEYPAD_ROW_2 GPIO_PIN_2 ///< The row beginning with 7.
#define KEYPAD_ROW_3 GPIO_PIN_3 ///< The row beginning with a symbol.
#define KEYPAD_ROWS (KEYPAD_ROW_0|KEYPAD_ROW_1|KEYPAD_ROW_2|KEYPAD_ROW_3)
	///< Handy for when you want to reference every pin connected to a row.

#define KEYPAD_COL_LEFT GPIO_PIN_4 ///< The column beginning with 1.
#define KEYPAD_COL_MID GPIO_PIN_5 ///< The column beginning with 2.
#define KEYPAD_COL_RIGHT GPIO_PIN_6 ///< The column beginning with 3.
#define KEYPAD_COLS (KEYPAD_COL_LEFT|KEYPAD_COL_MID|KEYPAD_COL_RIGHT)
	///< Handy for when you want to reference every pin connected to a column.

#define KEYPAD_ROW_COUNT 4 ///< The number of rows on the keypad.
#define KEYPAD_COL_COUNT 3 ///< The number of columns on the keypad.

#define LED_RED GPIO_PIN_1
#define LED_BLUE GPIO_PIN_2
#define LED_GREEN GPIO_PIN_3

static unsigned long keypad_port;
	///< The variable used to remember which port the keypad is connected to.

static int last_key_pressed = -2;
	///< The last key that was pressed. Initialized to "impossible" value.

static int keys[KEYPAD_ROW_COUNT][KEYPAD_COL_COUNT] = {
  {1,2,3},
	{4,5,6},
	{7,8,9},
	{-1,0,-1}
}; ///< The lookup table for what number is on which column & row.

/*----------------------------------------------------------------------------
	Initializes a port to use a keypad.
	
	This will always use pins 0-6 and leave pin 7 untouched.
*/
void initKeypad(unsigned long ulPort) {
	GPIOPinTypeGPIOOutput(
		ulPort,
		KEYPAD_COL_LEFT|KEYPAD_COL_RIGHT|KEYPAD_COL_MID
	);

	GPIOPinTypeGPIOInput(
		ulPort,
		KEYPAD_ROW_0|KEYPAD_ROW_1|KEYPAD_ROW_2|KEYPAD_ROW_3
	);

	GPIOPinWrite(GPIO_PORTB_BASE, KEYPAD_COLS, KEYPAD_COLS); // turn on every column
	// I do this so that when any key is pressed, a rising edge will happen on Port B and
	// trigger an interrupt.  The ISR polls column-by-column to see key in particular it is.

	keypad_port = ulPort;
}

/*----------------------------------------------------------------------------
	Checks to see which key (if any) on a given column is active.
	
	Assumes one keypad.
*/

int checkKeypadCol(unsigned char checkCol) {
	long rowPins;

	GPIOPinWrite(keypad_port, KEYPAD_COLS, checkCol);

	rowPins = GPIOPinRead(keypad_port, KEYPAD_ROWS);
	if (rowPins & KEYPAD_ROW_0) {
		return 0;
	} else if (rowPins & KEYPAD_ROW_1) {
		return 1;
	} else if (rowPins & KEYPAD_ROW_2) {
		return 2;
	} else if (rowPins & KEYPAD_ROW_3) {
		return 3;
	}
	return -1;
}


/*----------------------------------------------------------------------------
	Checks what number is being pressed on the keypad.
	
	Checks col-by-col, checks the upper-left first.
*/
int checkKeypad(void) {
	int row = 0;
	int col = 0;

	row = checkKeypadCol(KEYPAD_COL_LEFT);
	if (row != -1) {
		col = 0;
		return keys[row][col];
	}

	row = checkKeypadCol(KEYPAD_COL_MID);
	if (row != -1) {
		col = 1;
		return keys[row][col];
	}

	row = checkKeypadCol(KEYPAD_COL_RIGHT);
	if (row != -1) {
		col = 2;
		return keys[row][col];
	}
	return -1;
}

/*----------------------------------------------------------------------------
	Gets the last key pressed.
*/
int getLastKeyPressed(void) {
	return last_key_pressed;
}

void keyPressISR(void) {
	last_key_pressed = checkKeypad(); //poll the keypad
	if (last_key_pressed == -1) { // if a symbol was pressed
		GPIOPinWrite(GPIO_PORTF_BASE, LED_RED|LED_BLUE|LED_GREEN, 0); // turn off the LED
	} else {
		GPIOPinWrite(GPIO_PORTF_BASE, LED_RED|LED_BLUE|LED_GREEN, LED_GREEN); //otherwise, turn it on
	}
	GPIOPinWrite(GPIO_PORTB_BASE, KEYPAD_COLS, KEYPAD_COLS);
	// Then set the columns high so the interrupt can trigger again
}
