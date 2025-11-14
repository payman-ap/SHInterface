/***********************************************************************//**
 * @file		lpc17xx_spi.c
 * @brief		Contains all functions support for SPI firmware library on LPC17xx
 * @version		2.0
 * @date		21. May. 2010
 * @author		NXP MCU SW Application Team
 **************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 **********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup SPI
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_spi.h"
#include "lpc17xx_clkpwr.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */
#ifdef __BUILD_WITH_EXAMPLE__
#include "lpc17xx_libcfg.h"
#else
#include "lpc17xx_libcfg_default.h"
#endif /* __BUILD_WITH_EXAMPLE__ */


#define SSP_SR_TFE      ((uint32_t)(1<<0)) /** SSP status TX FIFO Empty bit */
#define SSP_SR_TNF      ((uint32_t)(1<<1)) /** SSP status TX FIFO not full bit */
#define SSP_SR_RNE      ((uint32_t)(1<<2)) /** SSP status RX FIFO not empty bit */
#define SSP_SR_RFF      ((uint32_t)(1<<3)) /** SSP status RX FIFO full bit */
#define SSP_SR_BSY      ((uint32_t)(1<<4)) /** SSP status SSP Busy bit */
#define SSP_SR_BITMASK	((uint32_t)(0x1F)) /** SSP SR bit mask */


/**
  * @brief  Initializes the SSP0.
  *
  * @param  None
  * @retval None 
  */
void SD_SPI_Init (void)
{
    /* Enable SSPI0 block */
    LPC_SC->PCONP |= (1UL << 21);
    
    /* Set SSEL0 as GPIO, output high */
    LPC_PINCON->PINSEL3 &= ~(3UL << 10);          /* Configure P1.21(SSEL) as GPIO */
    LPC_GPIO1->FIODIR |= (1UL << 21);            /* set P1.21 as output */
    
    /* Configure other SSP pins: SCK, MISO, MOSI */
    LPC_PINCON->PINSEL3 &= ~(3UL << 8);
    LPC_PINCON->PINSEL3 |=  (3UL << 8);          /* P1.20: SCK0 */
//    LPC_PINCON->PINSEL1 &= ~((3UL<<14) | (3UL<<16));
    LPC_PINCON->PINSEL3 |=  ((3UL<<14) | (3UL<<16));  /* P1.23: MISO0, P0.24: MOSI0 */
    
    /* Configure SSP0_PCLK to CCLK(100MHz), default value is CCLK/4 */
    LPC_SC->PCLKSEL1 &= ~(3UL << 10);
    LPC_SC->PCLKSEL1 |=  (1UL << 10);  /* SSP0_PCLK=CCLK */
    
    /* 8bit, SPI frame format, CPOL=0, CPHA=0, SCR=0 */  
    LPC_SSP0->CR0 = (0x07 << 0) |     /* data width: 8bit*/
                    (0x00 << 4) |     /* frame format: SPI */
                    (0x00 << 6) |     /* CPOL: low level */
                    (0x00 << 7) |     /* CPHA: first edge */
                    (0x00 << 8);      /* SCR = 0 */

    /* Enable SSP0 as a master */
    LPC_SSP0->CR1 = (0x00 << 0) |   /* Normal mode */
                    (0x01 << 1) |   /* Enable SSP0 */
                    (0x00 << 2) |   /* Master */
                    (0x00 << 3);    /* slave output disabled */

    /* Configure SSP0 clock rate to 400kHz (100MHz/250) */
    SPI_ConfigClockRate (SPI_CLOCKRATE_LOW);

    /* Set SSEL to high */
    SPI_CS_High ();
}

/**
  * @brief  Configure SSP0 clock rate.
  *
  * @param  SPI_CLOCKRATE: Specifies the SPI clock rate.
  *         The value should be SPI_CLOCKRATE_LOW or SPI_CLOCKRATE_HIGH.
  * @retval None 
  *
  * SSP0_CLK = CCLK / SPI_CLOCKRATE
  */
void SPI_ConfigClockRate (uint32_t SPI_CLOCKRATE)
{
    /* CPSR must be an even value between 2 and 254 */
    LPC_SSP0->CPSR = (SPI_CLOCKRATE & 0x1FE);    
}

/**
  * @brief  Set SSEL to low: select spi slave.
  *
  * @param  None.
  * @retval None 
  */
void SPI_CS_Low (void)
{
    /* SSEL is GPIO, set to high.  */
    LPC_GPIO0->FIOPIN &= ~(1 << 16);            
}

/**
  * @brief  Set SSEL to high: de-select spi slave.
  *
  * @param  None.
  * @retval None 
  */
void SPI_CS_High (void)
{
    /* SSEL is GPIO, set to high.  */
    LPC_GPIO0->FIOPIN |= (1 << 16);             
}

/**
  * @brief  Send one byte via MOSI and simutaniously receive one byte via MISO.
  *
  * @param  data: Specifies the byte to be sent out.
  * @retval Returned byte.
  *
  * Note: Each time send out one byte at MOSI, Rx FIFO will receive one byte. 
  */
uint8_t SPI_SendByte (uint8_t data)
{
    /* Put the data on the FIFO */
    LPC_SSP0->DR = data;
    /* Wait for sending to complete */
    while (LPC_SSP0->SR & SSP_SR_BSY); 
    /* Return the received value */              
    return (LPC_SSP0->DR);                        
}

/**
  * @brief  Receive one byte via MISO.
  *
  * @param  None.
  * @retval Returned received byte.
  */
uint8_t SPI_RecvByte (void)
{
    /* Send 0xFF to provide clock for MISO to receive one byte */
    return SPI_SendByte (0xFF);
}

/* SPI FIFO: from Martin Thomas's project */
#ifdef USE_FIFO

/* 8 frame FIFOs for both transmit and receive */
#define SSP_FIFO_DEPTH       8 

/**
  * @brief  Send data block using FIFO.
  *
  * @param  buf: Pointer to the byte array to be sent
  * @param  len: length (in byte) of the byte array.
  *              Should be multiple of 4.   
  * @retval None.
  */
void SPI_SendBlock_FIFO (const uint8_t *buf, uint32_t len)
{
	uint32_t cnt;
	uint16_t data;

	LPC_SSP0->CR0 |= 0x0f;  /* DSS to 16 bit */

	/* fill the FIFO unless it is full */
	for ( cnt = 0; cnt < ( len / 2 ); cnt++ ) 
	{
		/* wait for TX FIFO not full (TNF) */
		while ( !( LPC_SSP0->SR & SSP_SR_TNF) );
		
		data  = (*buf++) << 8;
		data |= *buf++;
		LPC_SSP0->DR = data;
	}

	/* wait for BSY gone */
	while ( LPC_SSP0->SR & SSP_SR_BSY);

	/* drain receive FIFO */
	while ( LPC_SSP0->SR & SSP_SR_RNE ) {
		data = LPC_SSP0->DR; 
	}

	LPC_SSP0->CR0 &= ~0x08;  /* DSS to 8 bit */    
}


/**
  * @brief  Receive data block using FIFO.
  *
  * @param  buf: Pointer to the byte array to store received data
  * @param  len: Specifies the length (in byte) to receive.
  *              Should be multiple of 4.   
  * @retval None.
  */
void SPI_RecvBlock_FIFO (uint8_t *buf, uint32_t len)
{
	uint32_t hwtr, startcnt, i, rec;

	hwtr = len/2;
	if ( len < SSP_FIFO_DEPTH ) {
		startcnt = hwtr;
	} else {
		startcnt = SSP_FIFO_DEPTH;
	}

	LPC_SSP0 -> CR0 |= 0x0f;  /* DSS to 16 bit */

	for ( i = startcnt; i; i-- ) {
		LPC_SSP0->DR = 0xffff;  // fill TX FIFO
	}

	do {
		while ( !(LPC_SSP0->SR & SSP_SR_RNE ) ) {
			// wait for data in RX FIFO (RNE set)
		}
		rec = LPC_SSP0->DR;
		if ( i < ( hwtr - startcnt ) ) {
			LPC_SSP0->DR = 0xffff;
		}
		*buf++ = (uint8_t)(rec >> 8);
		*buf++ = (uint8_t)(rec);
		i++;
	} while ( i < hwtr );

    LPC_SSP0->CR0 &= ~0x08;  /* DSS to 8 bit */
}
#endif

#ifdef _SPI
/* Public Functions ----------------------------------------------------------- */
/** @addtogroup SPI_Public_Functions
 * @{
 */

/*********************************************************************//**
 * @brief 		Setup clock rate for SPI device
 * @param[in] 	SPIx	SPI peripheral definition, should be LPC_SPI
 * @param[in]	target_clock : clock of SPI (Hz)
 * @return 		None
 ***********************************************************************/
void SPI_SetClock (LPC_SPI_TypeDef *SPIx, uint32_t target_clock)
{
	uint32_t spi_pclk;
	uint32_t prescale, temp;

	CHECK_PARAM(PARAM_SPIx(SPIx));

	if (SPIx == LPC_SPI){
		spi_pclk =  CLKPWR_GetPCLK (CLKPWR_PCLKSEL_SPI);
	} else {
		return;
	}

	prescale = 8;
	// Find closest clock to target clock
	while (1){
		temp = target_clock * prescale;
		if (temp >= spi_pclk){
			break;
		}
		prescale += 2;
		if(prescale >= 254){
			break;
		}
	}

	// Write to register
	SPIx->SPCCR = SPI_SPCCR_COUNTER(prescale);
}


/*********************************************************************//**
 * @brief		De-initializes the SPIx peripheral registers to their
*                  default reset values.
 * @param[in]	SPIx	SPI peripheral selected, should be LPC_SPI
 * @return 		None
 **********************************************************************/
void SPI_DeInit(LPC_SPI_TypeDef *SPIx)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));

	if (SPIx == LPC_SPI){
		/* Set up clock and power for SPI module */
		CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCSPI, DISABLE);
	}
}

/*********************************************************************//**
 * @brief		Get data bit size per transfer
 * @param[in]	SPIx	SPI peripheral selected, should be LPC_SPI
 * @return 		number of bit per transfer, could be 8-16
 **********************************************************************/
uint8_t SPI_GetDataSize (LPC_SPI_TypeDef *SPIx)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));
	return ((SPIx->SPCR)>>8 & 0xF);
}

/********************************************************************//**
 * @brief		Initializes the SPIx peripheral according to the specified
*               parameters in the UART_ConfigStruct.
 * @param[in]	SPIx	SPI peripheral selected, should be LPC_SPI
 * @param[in]	SPI_ConfigStruct Pointer to a SPI_CFG_Type structure
*                    that contains the configuration information for the
*                    specified SPI peripheral.
 * @return 		None
 *********************************************************************/
void SPI_Init(LPC_SPI_TypeDef *SPIx, SPI_CFG_Type *SPI_ConfigStruct)
{
	uint32_t tmp;

	CHECK_PARAM(PARAM_SPIx(SPIx));

	if(SPIx == LPC_SPI){
		/* Set up clock and power for UART module */
		CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCSPI, ENABLE);
	} else {
		return;
	}

	// Configure SPI, interrupt is disable as default
	tmp = ((SPI_ConfigStruct->CPHA) | (SPI_ConfigStruct->CPOL) \
		| (SPI_ConfigStruct->DataOrder) | (SPI_ConfigStruct->Databit) \
		| (SPI_ConfigStruct->Mode) | SPI_SPCR_BIT_EN) & SPI_SPCR_BITMASK;
	// write back to SPI control register
	SPIx->SPCR = tmp;

	// Set clock rate for SPI peripheral
	SPI_SetClock(SPIx, SPI_ConfigStruct->ClockRate);

	// If interrupt flag is set, Write '1' to Clear interrupt flag
	if (SPIx->SPINT & SPI_SPINT_INTFLAG){
		SPIx->SPINT = SPI_SPINT_INTFLAG;
	}
}



/*****************************************************************************//**
* @brief		Fills each SPI_InitStruct member with its default value:
* 				- CPHA = SPI_CPHA_FIRST
* 				- CPOL = SPI_CPOL_HI
* 				- ClockRate = 1000000
* 				- DataOrder = SPI_DATA_MSB_FIRST
* 				- Databit = SPI_DATABIT_8
* 				- Mode = SPI_MASTER_MODE
* @param[in]	SPI_InitStruct Pointer to a SPI_CFG_Type structure
*                    which will be initialized.
* @return		None
*******************************************************************************/
void SPI_ConfigStructInit(SPI_CFG_Type *SPI_InitStruct)
{
	SPI_InitStruct->CPHA = SPI_CPHA_FIRST;
	SPI_InitStruct->CPOL = SPI_CPOL_HI;
	SPI_InitStruct->ClockRate = 1000000;
	SPI_InitStruct->DataOrder = SPI_DATA_MSB_FIRST;
	SPI_InitStruct->Databit = SPI_DATABIT_8;
	SPI_InitStruct->Mode = SPI_MASTER_MODE;
}

/*********************************************************************//**
 * @brief		Transmit a single data through SPIx peripheral
 * @param[in]	SPIx	SPI peripheral selected, should be LPC_SPI
 * @param[in]	Data	Data to transmit (must be 16 or 8-bit long,
 * 						this depend on SPI data bit number configured)
 * @return 		none
 **********************************************************************/
void SPI_SendData(LPC_SPI_TypeDef* SPIx, uint16_t Data)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));

	SPIx->SPDR = Data & SPI_SPDR_BITMASK;
}



/*********************************************************************//**
 * @brief		Receive a single data from SPIx peripheral
 * @param[in]	SPIx	SPI peripheral selected, should be LPC_SPI
 * @return 		Data received (16-bit long)
 **********************************************************************/
uint16_t SPI_ReceiveData(LPC_SPI_TypeDef* SPIx)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));

	return ((uint16_t) (SPIx->SPDR & SPI_SPDR_BITMASK));
}

/*********************************************************************//**
 * @brief 		SPI 	Read write data function
 * @param[in]	SPIx 	Pointer to SPI peripheral, should be LPC_SPI
 * @param[in]	dataCfg	Pointer to a SPI_DATA_SETUP_Type structure that
 * 						contains specified information about transmit
 * 						data configuration.
 * @param[in]	xfType	Transfer type, should be:
 * 						- SPI_TRANSFER_POLLING: Polling mode
 * 						- SPI_TRANSFER_INTERRUPT: Interrupt mode
 * @return 		Actual Data length has been transferred in polling mode.
 * 				In interrupt mode, always return (0)
 * 				Return (-1) if error.
 * Note: This function can be used in both master and slave mode.
 ***********************************************************************/
int32_t SPI_ReadWrite (LPC_SPI_TypeDef *SPIx, SPI_DATA_SETUP_Type *dataCfg, \
						SPI_TRANSFER_Type xfType)
{
	uint8_t *rdata8;
    uint8_t *wdata8;
	uint16_t *rdata16;
    uint16_t *wdata16;
    uint32_t stat;
    uint32_t temp;
    uint8_t dataword;

	//read for empty buffer
	temp = SPIx->SPDR;
	//dummy to clear status
	temp = SPIx->SPSR;
	dataCfg->counter = 0;
	dataCfg->status = 0;

	if(SPI_GetDataSize (SPIx) == 8)
		dataword = 0;
	else dataword = 1;
	if (xfType == SPI_TRANSFER_POLLING){

		if (dataword == 0){
			rdata8 = (uint8_t *)dataCfg->rx_data;
			wdata8 = (uint8_t *)dataCfg->tx_data;
		} else {
			rdata16 = (uint16_t *)dataCfg->rx_data;
			wdata16 = (uint16_t *)dataCfg->tx_data;
		}

		while(dataCfg->counter < dataCfg->length)
		{
			// Write data to buffer
			if(dataCfg->tx_data == NULL){
				if (dataword == 0){
					SPI_SendData(SPIx, 0xFF);
				} else {
					SPI_SendData(SPIx, 0xFFFF);
				}
			} else {
				if (dataword == 0){
					SPI_SendData(SPIx, *wdata8);
					wdata8++;
				} else {
					SPI_SendData(SPIx, *wdata16);
					wdata16++;
				}
			}
			// Wait for transfer complete
			while (!((stat = SPIx->SPSR) & SPI_SPSR_SPIF));
			// Check for error
			if (stat & (SPI_SPSR_ABRT | SPI_SPSR_MODF | SPI_SPSR_ROVR | SPI_SPSR_WCOL)){
				// save status
				dataCfg->status = stat | SPI_STAT_ERROR;
				return (dataCfg->counter);
			}
			// Read data from SPI dat
			temp = (uint32_t) SPI_ReceiveData(SPIx);

			// Store data to destination
			if (dataCfg->rx_data != NULL)
			{
				if (dataword == 0){
					*(rdata8) = (uint8_t) temp;
					rdata8++;
				} else {
					*(rdata16) = (uint16_t) temp;
					rdata16++;
				}
			}
			// Increase counter
			if (dataword == 0){
				dataCfg->counter++;
			} else {
				dataCfg->counter += 2;
			}
		}

		// Return length of actual data transferred
		// save status
		dataCfg->status = stat | SPI_STAT_DONE;
		return (dataCfg->counter);
	}
	// Interrupt mode
	else {

		// Check if interrupt flag is already set
		if(SPIx->SPINT & SPI_SPINT_INTFLAG){
			SPIx->SPINT = SPI_SPINT_INTFLAG;
		}
		if (dataCfg->counter < dataCfg->length){
			// Write data to buffer
			if(dataCfg->tx_data == NULL){
				if (dataword == 0){
					SPI_SendData(SPIx, 0xFF);
				} else {
					SPI_SendData(SPIx, 0xFFFF);
				}
			} else {
				if (dataword == 0){
					SPI_SendData(SPIx, (*(uint8_t *)dataCfg->tx_data));
				} else {
					SPI_SendData(SPIx, (*(uint16_t *)dataCfg->tx_data));
				}
			}
			SPI_IntCmd(SPIx, ENABLE);
		} else {
			// Save status
			dataCfg->status = SPI_STAT_DONE;
		}
		return (0);
	}
}


/********************************************************************//**
 * @brief 		Enable or disable SPIx interrupt.
 * @param[in]	SPIx	SPI peripheral selected, should be LPC_SPI
 * @param[in]	NewState New state of specified UART interrupt type,
 * 				should be:
 * 				- ENALBE: Enable this SPI interrupt.
* 				- DISALBE: Disable this SPI interrupt.
 * @return 		None
 *********************************************************************/
void SPI_IntCmd(LPC_SPI_TypeDef *SPIx, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if (NewState == ENABLE)
	{
		SPIx->SPCR |= SPI_SPCR_SPIE;
	}
	else
	{
		SPIx->SPCR &= (~SPI_SPCR_SPIE) & SPI_SPCR_BITMASK;
	}
}


/********************************************************************//**
 * @brief 		Checks whether the SPI interrupt flag is set or not.
 * @param[in]	SPIx	SPI peripheral selected, should be LPC_SPI
 * @return 		The new state of SPI Interrupt Flag (SET or RESET)
 *********************************************************************/
IntStatus SPI_GetIntStatus (LPC_SPI_TypeDef *SPIx)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));

	return ((SPIx->SPINT & SPI_SPINT_INTFLAG) ? SET : RESET);
}

/********************************************************************//**
 * @brief 		Clear SPI interrupt flag.
 * @param[in]	SPIx	SPI peripheral selected, should be LPC_SPI
 * @return 		None
 *********************************************************************/
void SPI_ClearIntPending(LPC_SPI_TypeDef *SPIx)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));

	SPIx->SPINT = SPI_SPINT_INTFLAG;
}

/********************************************************************//**
 * @brief 		Get current value of SPI Status register in SPIx peripheral.
 * @param[in]	SPIx	SPI peripheral selected, should be LPC_SPI
 * @return		Current value of SPI Status register in SPI peripheral.
 * Note:	The return value of this function must be used with
 * 			SPI_CheckStatus() to determine current flag status
 * 			corresponding to each SPI status type. Because some flags in
 * 			SPI Status register will be cleared after reading, the next reading
 * 			SPI Status register could not be correct. So this function used to
 * 			read SPI status register in one time only, then the return value
 * 			used to check all flags.
 *********************************************************************/
uint32_t SPI_GetStatus(LPC_SPI_TypeDef* SPIx)
{
	CHECK_PARAM(PARAM_SPIx(SPIx));

	return (SPIx->SPSR & SPI_SPSR_BITMASK);
}

/********************************************************************//**
 * @brief 		Checks whether the specified SPI Status flag is set or not
 * 				via inputSPIStatus parameter.
 * @param[in]	inputSPIStatus Value to check status of each flag type.
 * 				This value is the return value from SPI_GetStatus().
 * @param[in]	SPIStatus	Specifies the SPI status flag to check,
 * 				should be one of the following:
				- SPI_STAT_ABRT: Slave abort.
				- SPI_STAT_MODF: Mode fault.
				- SPI_STAT_ROVR: Read overrun.
				- SPI_STAT_WCOL: Write collision.
				- SPI_STAT_SPIF: SPI transfer complete.
 * @return 		The new state of SPIStatus (SET or RESET)
 *********************************************************************/
FlagStatus SPI_CheckStatus (uint32_t inputSPIStatus,  uint8_t SPIStatus)
{
	CHECK_PARAM(PARAM_SPI_STAT(SPIStatus));

	return ((inputSPIStatus & SPIStatus) ? SET : RESET);
}





/**
 * @}
 */

#endif /* _SPI */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
