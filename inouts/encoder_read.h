/******************************************************************
 *****                                                        *****
 *****  Name: encoder_read.h                                  *****
 *****  Ver.: 1.0                                             *****
 *****  Date: 28/09/2016                                      *****
 *****  Auth: Payman Azaripasand                              *****
 *****  Func: implements a dynamic Software-Hardware          *****
 *****        Interface to be used in Matlab Simulink         *****
 *****                                                        *****
 ******************************************************************/

#ifndef QENCODER_H
#define QENCODER_H

#include "lpc17xx_timer.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"

#include "initparams.h"


//Public Variables


//Public Functions
void init_qencoder(void);
short get_qenc_value(uint8_t channel);
void reset_qencoder(uint8_t channel);

//Private Functions
void qenc_channel_begin(uint8_t channel);
void TIMER_IRQHandler(QENC1_TIM);
void TIMER_IRQHandler(QENC3_TIM);
void TIMER_IRQHandler(QENC5_TIM);


#endif
