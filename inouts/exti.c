/******************************************************************
 *****                                                        *****
 *****  Name: exti.c                                          *****
 *****  Ver.: 1.0                                             *****
 *****  Date: 28/09/2016                                      *****
 *****  Auth: Payman Azaripasand                              *****
 *****  Func: implements a dynamic Software-Hardware          *****
 *****        Interface to be used in Matlab Simulink         *****
 *****                                                        *****
 ******************************************************************/

#include "exti.h"
#include "lpc17xx_pinsel.h"


void init_ext_interrupts()
{

	PINSEL_CFG_Type PinCfg;
	EXTI_InitTypeDef EXTICfg;
	/* Initialize EXT pin and register */
	/* P2.12 as /EINT2 */
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 11;
	PinCfg.Portnum = 2;
	PINSEL_ConfigPin(&PinCfg);

	EXTI_Init();

	EXTICfg.EXTI_Line = EXTI_EINT1;
	/* edge sensitive */
	EXTICfg.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
	EXTICfg.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;
	EXTI_ClearEXTIFlag(EXTI_EINT1);
	EXTI_Config(&EXTICfg);

	NVIC_SetPriorityGrouping(4);
	NVIC_SetPriority(EINT1_IRQn, 0);
	NVIC_EnableIRQ(EINT1_IRQn);
}


void EINT1_IRQHandler(void)
{
	//EXTI_InitTypeDef EXTICfg;

	EXTI_ClearEXTIFlag(EXTI_EINT1);
	//EXTICfg.EXTI_Line = EXTI_EINT1;

	//reset_uart_reciever();
	if(isConfigFlag == FALSE)
		isConfigFlag = TRUE;
	else
		isConfigFlag = FALSE;

	//EXTI_Config(&EXTICfg);
}
