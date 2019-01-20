/*
             LUFA Library
     Copyright (C) Dean Camera, 2017.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2017  Dean Camera (dean [at] fourwalledcubicle [dot] com)
  Copyright 2010  Denver Gingerich (denver [at] ossguy [dot] com)

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
 *  Header file for Descriptors.c.
 */

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

	/* Includes: */
		#include <LUFA/Drivers/USB/USB.h>

		#include <avr/pgmspace.h>

	/* Type Defines: */
		/** Type define for the device configuration descriptor structure. This must be defined in the
		 *  application code, as the configuration descriptor contains several sub-descriptors which
		 *  vary between devices, and which describe the device's usage to the host.
		 */
		typedef struct
		{
			USB_Descriptor_Configuration_Header_t Config;

			// BUTTON Interface
			USB_Descriptor_Interface_t            BUTTON_Interface;
			USB_Descriptor_Endpoint_t             PAD_ReportIN_HAUT;
			USB_Descriptor_Endpoint_t             PAD_ReportIN_BAS;
			USB_Descriptor_Endpoint_t             PAD_ReportIN_GAUCHE;
			USB_Descriptor_Endpoint_t             PAD_ReportIN_DROITE;
			USB_Descriptor_Endpoint_t             PAD_ReportIN_JOYSTICK;

			USB_Descriptor_Interface_t            LED_Interface;
			USB_Descriptor_Endpoint_t             PAD_ReportOUT_LED0
			USB_Descriptor_Endpoint_t             PAD_ReportOUT_LED1
			USB_Descriptor_Endpoint_t             PAD_ReportOUT_LED2
			USB_Descriptor_Endpoint_t             PAD_ReportOUT_LED3
			USB_Descriptor_Endpoint_t             PAD_ReportOUT_LED4
			USB_Descriptor_Endpoint_t             PAD_ReportOUT_LED5
		} USB_Descriptor_Configuration_t;

		/** Enum for the device interface descriptor IDs within the device. Each interface descriptor
		 *  should have a unique ID index associated with it, which can be used to refer to the
		 *  interface from other descriptors.
		 */
		enum InterfaceDescriptors_t
		{
			INTERFACE_ID_BUTTON = 1, /**<  BUTTON interface descriptor ID */
			INTERFACE_ID_LED = 2, /**< LEDS interface descriptor ID */
			
		};

		/** Enum for the device string descriptor IDs within the device. Each string descriptor should
		 *  have a unique ID index associated with it, which can be used to refer to the string from
		 *  other descriptors.
		 */
		enum StringDescriptors_t
		{
			STRING_ID_Language     = 0, /**< Supported Languages string descriptor ID (must be zero) */
			STRING_ID_Manufacturer = 1, /**< Manufacturer string ID */
			STRING_ID_Product      = 2, /**< Product string ID */

		};

	/* Macros: */
		/** Endpoint address of the PAD HID reporting IN endpoint. */
		#define PAD_OUT_LED0		 (ENDPOINT_DIR_OUT | 1)
		#define PAD_OUT_LED1		 (ENDPOINT_DIR_OUT | 2)
		#define PAD_OUT_LED2         (ENDPOINT_DIR_OUT | 3)
		#define PAD_OUT_LED3         (ENDPOINT_DIR_OUT | 4)
		#define PAD_OUT_LED4         (ENDPOINT_DIR_OUT | 5)
		#define PAD_OUT_LED5         (ENDPOINT_DIR_OUT | 6)

		/** Endpoint address of the PAD HID reporting OUT endpoint. */
		#define PAD_IN_HAUT          (ENDPOINT_DIR_OUT | 7)
		#define PAD_IN_BAS           (ENDPOINT_DIR_OUT | 8)
		#define PAD_IN_GAUCHE        (ENDPOINT_DIR_OUT | 9)
		#define PAD_IN_DROITE        (ENDPOINT_DIR_OUT | 10)
		#define PAD_IN_JOYSTICK      (ENDPOINT_DIR_OUT | 11)


		/** Size in bytes of the PAD HID reporting IN and OUT endpoints. */
		#define PAD_EPSIZE           8 

	/* Function Prototypes: */
		uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
		                                    const uint16_t wIndex,
		                                    const void** const DescriptorAddress)
		                                    ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif

