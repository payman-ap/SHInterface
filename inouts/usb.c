/*----------------------------------------------------------------------------
 *      U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usb.c
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

#include "usb.h"

//Private Variables
UART_RING_BUFFER_T rbusb; // UART Ring buffer
__IO FlagStatus tx_usb_int; // Current Tx Interrupt enable state

//unsigned char *pDat = (unsigned char*)(&num);


void init_usb(UART_CFG_Type usb_config)
{
    // UART Configuration structure variable
//    UART_CFG_Type UARTConfigStruct;
    // UART FIFO configuration Struct variable
    UART_FIFO_CFG_Type UARTFIFOConfigStruct;
    // Pin configuration for UART
    PINSEL_CFG_Type PinCfg;

    //TODO UART0 has a different struct type LPC_UART0_TypeDef
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
    PinCfg.Funcnum = USB_FCN;
	PinCfg.Portnum = USB_PRT;
	PinCfg.Pinnum = USB_RX_PIN;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = USB_TX_PIN;
	PINSEL_ConfigPin(&PinCfg);


    // Initialize UART peripheral with given to corresponding parameter
    UART_Init((LPC_UART_TypeDef *)LPC_UART(USB_UART), &usb_config);


	/* Initialize FIFOConfigStruct to default state:
	*              - FIFO_DMAMode = DISABLE
	*              - FIFO_Level = UART_FIFO_TRGLEV0
	*              - FIFO_ResetRxBuf = ENABLE
	*              - FIFO_ResetTxBuf = ENABLE
	*              - FIFO_State = ENABLE
	*/
    UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

    // Initialize FIFO for UART peripheral
    UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART(USB_UART), &UARTFIFOConfigStruct);


    // Enable UART Transmit
    UART_TxCmd((LPC_UART_TypeDef *)LPC_UART(USB_UART), ENABLE);

    /* Enable UART Rx interrupt */
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART(USB_UART), UART_INTCFG_RBR, ENABLE);
    /* Enable UART line status interrupt */
    //    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART(USB_UART), UART_INTCFG_RLS, ENABLE);
    /*
     * Do not enable transmit interrupt here, since it is handled by
     * UART_Send() function, just to reset Tx Interrupt state for the
     * first time
     */
    tx_usb_int = RESET;

    // Reset ring buf head and tail idx
    __BUF_RESET(rbusb.rx_head);
    __BUF_RESET(rbusb.rx_tail);
    __BUF_RESET(rbusb.tx_head);
    __BUF_RESET(rbusb.tx_tail);

    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(UART_IRQn(USB_UART), ((0x01<<3)|0x01));
    /* Enable Interrupt for UART channel */
    NVIC_EnableIRQ(UART_IRQn(USB_UART));

}

/*********************************************************************//**
 * @brief       UART interrupt handler sub-routine
 * @param[in]   None
 * @return      None
 **********************************************************************/
void UART_IRQHandler(USB_UART)
{
    uint32_t intsrc, tmp, tmp1;

    /* Determine the interrupt source */
    intsrc = UART_GetIntId((LPC_UART_TypeDef*)LPC_UART(USB_UART));

    tmp = intsrc & UART_IIR_INTID_MASK;

    // Receive Line Status
    if (tmp == UART_IIR_INTID_RLS)
    {
        // Check line status
        tmp1 = UART_GetLineStatus((LPC_UART_TypeDef*)LPC_UART(USB_UART));
        // Mask out the Receive Ready and Transmit Holding empty status
        tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
                | UART_LSR_BI | UART_LSR_RXFE);
        // If any error exist
        if (tmp1)
        {
//        	UART_IntErr_Call(USB_UART)(tmp1);
        	UART_IntErr(USB_UART)(tmp1);
        }
    }

    // Receive Data Available or Character time-out
    if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI))
    {
//    	UART_IntReceive_Call(USB_UART);
    	UART_IntReceive(USB_UART);
    }

    // Transmit Holding Empty
    if (tmp == UART_IIR_INTID_THRE)
    {
//    	UART_IntTransmit_Call(USB_UART);
    	UART_IntTransmit(USB_UART);
    }
}
/********************************************************************//**
 * @brief       UART receive function
 * @param[in]   None
 * @return      None
 *********************************************************************/
void UART_IntReceive(USB_UART)
{
	uint8_t tmpc;
	uint32_t rLen;
	while(1)
	{
		// Call UART read function in UART driver
		rLen = UART_Receive((LPC_UART_TypeDef *)LPC_UART(USB_UART), &tmpc, 1, NONE_BLOCKING);

		// If data received
		if (rLen>0)
		{
			//TODO Remove
//			UART_Send((LPC_UART_TypeDef *)LPC_UART(AHRS_UART), &tmpc, 1, NONE_BLOCKING);

		}
		else //no more data
		{
			break;
		}
	}
}

/********************************************************************//**
 * @brief       UART transmit function (ring buffer used)
 * @param[in]   None
 * @return      None
 *********************************************************************/
void UART_IntTransmit(USB_UART)
{
    // Disable THRE interrupt
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART(USB_UART), UART_INTCFG_THRE, DISABLE);

    /* Wait for FIFO buffer empty, transfer UART_TX_FIFO_SIZE bytes
     * of data or break whenever ring buffers are empty */
    /* Wait until THR empty */
    while (UART_CheckBusy((LPC_UART_TypeDef *)LPC_UART(USB_UART)) == SET);

    while (!__BUF_IS_EMPTY(rbusb.tx_head,rbusb.tx_tail))
    {
        /* Move a piece of data into the transmit FIFO */
        if (UART_Send((LPC_UART_TypeDef *)LPC_UART(USB_UART), (uint8_t *)&rbusb.tx[rbusb.tx_tail], 1, NONE_BLOCKING))
        {
        /* Update transmit ring FIFO tail pointer */
        __BUF_INCR(rbusb.tx_tail);
        } else {
            break;
        }
    }

    /* If there is no more data to send, disable the transmit
       interrupt - else enable it or keep it enabled */
    if (__BUF_IS_EMPTY(rbusb.tx_head, rbusb.tx_tail))
    {
        UART_IntConfig((LPC_UART_TypeDef *)LPC_UART(USB_UART), UART_INTCFG_THRE, DISABLE);
        // Reset Tx Interrupt state
        tx_usb_int = RESET;
    }
    else
    {
        // Set Tx Interrupt state
        tx_usb_int = SET;
        UART_IntConfig((LPC_UART_TypeDef *)LPC_UART(USB_UART), UART_INTCFG_THRE, ENABLE);
    }
}

/*********************************************************************//**
 * @brief       UART Line Status Error
 * @param[in]   bLSErrType  UART Line Status Error Type
 * @return      None
 **********************************************************************/
void UART_IntErr(USB_UART)(uint8_t bLSErrType)
{
    uint8_t test=0;

    // Avoiding Warning
    test=2*test;
    // Loop forever
    while (1){
        // For testing purpose
        test = bLSErrType;
    }
}


