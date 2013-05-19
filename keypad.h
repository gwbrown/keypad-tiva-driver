/*****************************************************************************
	Copyright (c) 2013 Gordon Brown. All rights reserved.
******************************************************************************

	File           - Description: Keypad Library 
	Version Number - Revision: 1
	Last Updated   - Date: 1/26/2013
	Updated By     - Author: Gordon Brown 
	Authored By    - Gordon Brown
*/

/** @file
	\brief Driver library for the Adafruit flexible keypad. This is currently
	build around a looping-polling type approach, and will soon be 
	rewritten for an interrupt-based approach.
	
	This driver assumes that only one button will be pressed at once. If 
	multiple buttons are pressed at once, the leftmost one (in the column with
	the 1 key on it) will be the one that registers.  If all pressed buttons
	are in the same column, then the topmost one will be the one to register.

	This library assumes that the keypad is attached to pins 0 through 6 on a
	single port, but it should be able to use any port with pins 0 through 6
	available.

	Warning: This library assumes only one keypad. If initKeypad is run more
	than once, the functions in this library will only recognize the keypad
	attached to the port most recently fed to initKeypad.

*/

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

/*--------------------------------------------------------------------------*/
/** Initializes the keypad port.

	Sets columns to output, rows to input with internal pull-downs. Uses pins
	0-6 and leaves pin 7 on the port alone.

	@param ulPort The port the keypad is connected to.

	@return No return value.
*/
void initKeypad(unsigned long ulPort);

/*--------------------------------------------------------------------------*/
/** Checks to see which key (if any) on a given column is active.

	Drives one column high, then checks each row to see if they have been
	driven high.

	@param checkCol The column to check.

	@return The row that's active on the given column. Returns -1 if
	no row is active.
*/
int checkKeypadCol(unsigned char checkCol);

/*--------------------------------------------------------------------------*/
/** Checks every column/row of the keypad and returns the key being pressed.

	Runs checkKeypadCol on every column until it finds a pressed key, then
	looks up the value for that column/row combination in a table.

	@see checkKeypadCol()

	@return The number of the key pressed.  Symbols # and * each return -1.
*/
int checkKeypad(void);

/*--------------------------------------------------------------------------*/
/** Gets the last key pressed.

    Call to get the last key that was pressed.

    @return The last key pressed.
*/
int getLastKeyPressed(void);

/*--------------------------------------------------------------------------*/
/** ISR for when a key is pressed.

    Currently saves the key that was pressed, then toggles an LED.
*/
void keyPressISR(void);

#endif /* KEYPAD_H_ */
