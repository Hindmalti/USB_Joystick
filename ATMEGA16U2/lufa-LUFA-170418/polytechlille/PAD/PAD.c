/*
             LUFA Library
     Copyright (C) Dean Camera, 2017.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2010  OBinou (obconseil [at] gmail [dot] com)
  Copyright 2017  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Main source file for the RelayBoard program. This file contains the main tasks of
 *  the project and is responsible for the initial application hardware configuration.
 */

#include "RelayBoard.h"



uint8_t data_send;
uint8_t state_send;

ISR(USART1_RX_vect)
{
	data_send = UDR1;
	state_send = 1;
}


/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	SetupHardware();
	
	GlobalInterruptEnable();

	for (;;)
	  USB_USBTask();
	  task_button_led();
}
void ReceiveNextReport(void)
{	
	Endpoint_SelectEndpoint(PAD_OUT_EPADDR1);
	if(Endpoint_IsOUTReceived())
	{
		if(Endpoint_IsReadWriteAllowed())
		{
			uint8_t value Endpoint_Read_8();
			Serial_SendByte(value & LED13_MASK);
			Endpoint_ClearIN();
		}
		Endpoint_SelectEndpoint(PAD_OUT_EPADDR2);
		if(Endpoint_IsReadWriteAllowed())
		{
			uint8_t value Endpoint_Read_8();
			Serial_SendByte(value & OTHER_LEDS_MASK);
			Endpoint_ClearIN();
		}
	}
}

/** Sends the next HID report to the host, via the keyboard data endpoint. */
void SendNextReport(void)
{
	if(state_send) // Si on a quelque chose à envoyer
	{
		// Pour le premier endoint = Boutons 3 à 6
		Endpoint_SelectEndpoint(PAD_IN_EPADDR1);
		if(Endpoint_IsReadWriteAllowed())
		{
			data_send = data_send & BUTTONS_MASK; // On passe la variable data_send sur un mask pour selectionner les boutons de 3 à 6 et pas le joystick
			Endpoint_Write_Stream_LE(&data_send, sizeof(uint8_t), NULL)
			Endpoint_ClearIN();
		}
		// Pour le premier endoint = Boutons 2 (joystick)
		Endpoint_SelectEndpoint(PAD_IN_EPADDR2);
		if(Endpoint_IsReadWriteAllowed())
		{
			data_send = data_send & JOYSTICK_MASK;
			Endpoint_Write_Stream_LE(&data_send, sizeof(uint8_t), NULL)
			Endpoint_ClearIN();
		}
	}

	state_send = 0;
	
}

void task_button_led(){
	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
	  return;

	/* Send the next keypress report to the host */
	SendNextReport();

	/* Process the LED report sent from the host */
	ReceiveNextReport();
}


}

/** Configures the board hardware and chip peripherals for the project's functionality. */
void SetupHardware(void)
{
	Serial_Init(BAUDRATE, DOUBLESPEED);
	UCSR1B |= (1 << RXCIE1);
	USB_Init();
		
}

