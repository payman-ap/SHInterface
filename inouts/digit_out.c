/******************************************************************
 *****                                                        *****
 *****  Name: digit_out.c                                     *****
 *****  Ver.: 1.0                                             *****
 *****  Date: 28/09/2016                                      *****
 *****  Auth: Payman Azaripasand                              *****
 *****  Func: implements a dynamic Software-Hardware          *****
 *****        Interface to be used in Matlab Simulink         *****
 *****                                                        *****
 ******************************************************************/

#include "digit_out.h"

extern uint8_t DI_PORTS[];
extern uint8_t DI_PINS[];

extern uint8_t DO_PORTS[];
extern uint8_t DO_PINS[];

void init_DIO()
{
	uint8_t i=0;
	/* Enable GPIO Clock */
	CLKPWR_ConfigPPWR(CLKPWR_PCONP_PCGPIO, ENABLE);

	for(i=0; i<=DI_MAX_INDEX; i++)
		GPIO_SetDir(DI_PORTS[i], ((uint32_t)1)<<DI_PINS[i], 0);

	for(i=0; i<=DO_MAX_INDEX; i++)
	{
		GPIO_SetDir(DO_PORTS[i], ((uint32_t)1)<<DO_PINS[i], 1);
		GPIO_ClearValue(DO_PORTS[i], ((uint32_t)1)<<DO_PINS[i]);
	}

}

uint8_t read_DI(uint8_t channel)
{
	uint32_t port_status = 0;
	uint32_t value;
	port_status=GPIO_ReadValue(DI_PORTS[channel]);
	value = (uint32_t)1;
	value = value << DI_PINS[channel];
	value = port_status & value;
	value = value > 0;
	return value;
}

void write_DO(uint8_t channel, uint8_t val)
{
	if(val)
		GPIO_SetValue(DO_PORTS[channel], ((uint32_t)1)<<DO_PINS[channel]);
	else
		GPIO_ClearValue(DO_PORTS[channel], ((uint32_t)1)<<DO_PINS[channel]);
}
