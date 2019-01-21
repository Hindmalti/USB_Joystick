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

#include "PAD.h"

uint8_t data_send;
uint8_t state_send;

ISR(USART1_RX_vect)
{
	data_send = UDR1;
	state_send = 1;
}

/*

*/


int main(void)
{
	SetupHardware();
	
	GlobalInterruptEnable();

	for (;;) {
		USB_USBTask(); 
		task_button_led();
	}
}

/*
Fonction qui recoit de la machine l'octet envoyé en série permettant d'allumer les LEDS
*/
void ReceiveNextReport(void)
{	
	/*
	Endpoint LEDS
	*/
	Endpoint_SelectEndpoint(PAD_OUT_LEDS);
	if(Endpoint_IsOUTReceived())
	{
		if(Endpoint_IsReadWriteAllowed())
		{
			char value = Endpoint_Read_8();
			Serial_SendByte(value);
			Endpoint_ClearIN();
		}
	}

}

/** Gère l'envoie à la machine hôte, ici l'état des boutons */
void SendNextReport(void)
{
	/*
	0 0 1 1 1 1 1 0
	*/
	if(state_send) // Si on a quelque chose à envoyer
	{
		/*
		Cas du Joystick
		*/
		Endpoint_SelectEndpoint(PAD_IN_BOUTONS)
		if(Endpoint_IsReadWriteAllowed())
		{
			Endpoint_Write_Stream_LE(&data_send, sizeof(uint8_t), NULL)
			Endpoint_ClearIN();
		}
	}

	state_send = 0;
	
}

void task_button_led(){
	/* Le device doit être connecté et configuré */
	if (USB_DeviceState != DEVICE_STATE_Configured)
	  return;

	/* Envoie le report des boutons à la machine hôte */
	SendNextReport();

	/* Procède au traitement des LEDS */
	ReceiveNextReport();
}



/** Initialise les registres du microcontrolleur afin de permettre les interruptions séries */
void SetupHardware(void)
{
	Serial_Init(BAUDRATE, DOUBLESPEED);
	UCSR1B |= (1 << RXCIE1);
	USB_Init();
		
}

