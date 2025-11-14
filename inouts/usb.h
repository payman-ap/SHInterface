/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usb.h
 * Purpose: USB Module
 * Version: V1.20
 *----------------------------------------------------------------------------
 *      This software is supplied "AS IS" without any warranties, express,
 *      implied or statutory, including but not limited to the implied
 *      warranties of fitness for purpose, satisfactory quality and
 *      noninfringement. Keil extends you a royalty-free right to reproduce
 *      and distribute executable files created using this software for use
 *      on NXP Semiconductors LPC family microcontroller devices only. Nothing 
 *      else gives you the right to use this software.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------
 * History:
 *          V1.20 Added vendor specific requests
 *                Changed string descriptor handling
 *                Reworked Endpoint0
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/

#ifndef USB_H
#define USB_H

#include "initparams.h"

//Public Functions
void init_usb(UART_CFG_Type);

//Private Functions
void UART_IRQHandler(USB_UART);
void UART_IntReceive(USB_UART);
void UART_IntTransmit(USB_UART);
void UART_IntErr(USB_UART)(uint8_t);

#endif
