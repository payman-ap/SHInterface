/******************************************************************
 *****                                                        *****
 *****  Name: exti.h                                          *****
 *****  Ver.: 1.0                                             *****
 *****  Date: 28/09/2016                                      *****
 *****  Auth: Payman Azaripasand                              *****
 *****  Func: implements a dynamic Software-Hardware          *****
 *****        Interface to be used in Matlab Simulink         *****
 *****                                                        *****
 ******************************************************************/

#ifndef EXTI_H
#define EXTI_H

#include "lpc17xx_exti.h"

extern Bool isConfigFlag;

void init_ext_interrupts(void);
void EINT1_IRQHandler(void);

#endif
