/******************************************************************
 *****                                                        *****
 *****  Name: digit_out.h                                     *****
 *****  Ver.: 1.0                                             *****
 *****  Date: 28/09/2016                                      *****
 *****  Auth: Payman Azaripasand                              *****
 *****  Func: implements a dynamic Software-Hardware          *****
 *****        Interface to be used in Matlab Simulink         *****
 *****                                                        *****
 ******************************************************************/

#ifndef DIO_H
#define DIO_H

#include "lpc17xx_gpio.h"
#include "lpc17xx_clkpwr.h"

#include "initparams.h"

void init_DIO(void);

uint8_t read_DI(uint8_t channel);
void write_DO(uint8_t channel, uint8_t val);

#endif
