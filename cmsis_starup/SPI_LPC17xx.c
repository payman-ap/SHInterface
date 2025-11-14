/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    SPI_LPC17xx.c
 *      Purpose: Serial Peripheral Interface Driver for NXP LPC17xx
 *      Rev.:    V4.10
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2010 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <File_Config.h>
#include <LPC17xx.H>                 /* LPC17xx Definitions                  */

/* SSPxSR - bit definitions. */
#define TFE     0x01
#define TNF     0x02
#define RNE     0x04
#define RFF     0x08
#define BSY     0x10

/*----------------------------------------------------------------------------
 *      SPI Driver Functions
 *----------------------------------------------------------------------------
 *  Required functions for SPI driver module:
 *   - void spi_init     ()
 *   - void spi_ss       (U32 ss)
 *   - U8   spi_send     (U8 outb)
 *   - void spi_hi_speed (BOOL on)
 *---------------------------------------------------------------------------*/

/*--------------------------- spi_init --------------------------------------*/

void spi_init (void) {
  /* Initialize and enable the SSP Interface module. */

  LPC_SC->PCONP       |= (1 << 21);           /* Enable power to SSPI0 block */

  /* SSEL is GPIO, output set to high. */
  LPC_GPIO0->FIODIR   |=  (1<<16);            /* P0.16 is output             */
  LPC_GPIO0->FIOPIN   |=  (1<<16);            /* set P0.16 high (SSEL inact.)*/
  LPC_PINCON->PINSEL1 &= ~(3<<0);             /* P0.16 SSEL (used as GPIO)   */

  /* SCK, MISO, MOSI are SSP pins. */
  LPC_PINCON->PINSEL0 &= ~(3UL<<30);          /* P0.15 cleared               */
  LPC_PINCON->PINSEL0 |=  (2UL<<30);          /* P0.15 SCK0                  */
  LPC_PINCON->PINSEL1 &= ~((3<<2) | (3<<4));  /* P0.17, P0.18 cleared        */
  LPC_PINCON->PINSEL1 |=  ((2<<2) | (2<<4));  /* P0.17 MISO0, P0.18 MOSI0    */

  LPC_SC->PCLKSEL1    &= ~(3<<10);            /* PCLKSP0 = CCLK/4 ( 25MHz)   */
  LPC_SC->PCLKSEL1    |=  (1<<10);            /* PCLKSP0 = CCLK   (100MHz)   */

  LPC_SSP0->CPSR       = 250;                 /* 100MHz / 250 = 400kBit      */
                                              /* maximum of 18MHz is possible*/    
  LPC_SSP0->CR0        = 0x0007;              /* 8Bit, CPOL=0, CPHA=0        */
  LPC_SSP0->CR1        = 0x0002;              /* SSP0 enable, master         */
}


/*--------------------------- spi_hi_speed ----------------------------------*/

void spi_hi_speed (BOOL on) {
  /* Set a SPI clock to low/high speed for SD/MMC. */

  if (on == __TRUE) {
    /* Max. 12 MBit used for Data Transfer. */
    LPC_SSP0->CPSR = 10;                      /* 100MHz / 10 = 10MBit        */
  }
  else {
    /* Max. 400 kBit used in Card Initialization. */
    LPC_SSP0->CPSR = 250;                     /* 100MHz / 250 = 400kBit      */
  }
}


/*--------------------------- spi_ss ----------------------------------------*/

void spi_ss (U32 ss) {
  /* Enable/Disable SPI Chip Select (drive it high or low). */

  if (ss) {
    LPC_GPIO0->FIOPIN |= (1<<16);             /* SSEL is GPIO, set to high.  */
  }
  else {
    LPC_GPIO0->FIOPIN &= ~(1<<16);            /* SSEL is GPIO, set to high.  */
  }
}


/*--------------------------- spi_send --------------------------------------*/

U8 spi_send (U8 outb) {
  /* Write and Read a byte on SPI interface. */

  LPC_SSP0->DR = outb;
  while (LPC_SSP0->SR & BSY);                 /* Wait for transfer to finish */
  return (LPC_SSP0->DR);                      /* Return received value       */
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
