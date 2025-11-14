/******************************************************************
 *****                                                        *****
 *****  Name: analog_to_digit_conv.h                          *****
 *****  Ver.: 1.0                                             *****
 *****  Date: 28/09/2016                                      *****
 *****  Auth: Payman Azaripasand                              *****
 *****  Func: implements a dynamic Software-Hardware          *****
 *****        Interface to be used in Matlab Simulink         *****
 *****                                                        *****
 ******************************************************************/

#ifndef ADC_H
#define ADC_H

#include "lpc_types.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_pinsel.h"

#include "initparams.h"

//Private Variables

//Public Variables

//Public Functions
void init_adc(void);
uint16_t get_adc_value_channel(uint8_t channel);

//Private Functions

#endif
