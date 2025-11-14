/******************************************************************
 *****                                                        *****
 *****  Name: analog_to_digit_conv.c                          *****
 *****  Ver.: 1.0                                             *****
 *****  Date: 28/09/2016                                      *****
 *****  Auth: Payman Azaripasand                              *****
 *****  Func: implements a dynamic Software-Hardware          *****
 *****        Interface to be used in Matlab Simulink         *****
 *****                                                        *****
 ******************************************************************/

#include "analog_to_digit_conv.h"

void init_adc()
{
	PINSEL_CFG_Type PinCfg;

	//ADC0
	PinCfg.Funcnum = ADC0_FCN;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = ADC0_PIN;
	PinCfg.Portnum = ADC0_PRT;
	PINSEL_ConfigPin(&PinCfg);

	//ADC1
	PinCfg.Funcnum = ADC1_FCN;
	PinCfg.Pinnum = ADC1_PIN;
	PinCfg.Portnum = ADC1_PRT;
	PINSEL_ConfigPin(&PinCfg);

	//ADC2
	PinCfg.Funcnum = ADC2_FCN;
	PinCfg.Pinnum = ADC2_PIN;
	PinCfg.Portnum = ADC2_PRT;
	PINSEL_ConfigPin(&PinCfg);

	//ADC3
	PinCfg.Funcnum = ADC3_FCN;
	PinCfg.Pinnum = ADC3_PIN;
	PinCfg.Portnum = ADC3_PRT;
	PINSEL_ConfigPin(&PinCfg);

	//ADC4
	PinCfg.Funcnum = ADC4_FCN;
	PinCfg.Pinnum = ADC4_PIN;
	PinCfg.Portnum = ADC4_PRT;
	PINSEL_ConfigPin(&PinCfg);

	//ADC5
	PinCfg.Funcnum = ADC5_FCN;
	PinCfg.Pinnum = ADC5_PIN;
	PinCfg.Portnum = ADC5_PRT;
	PINSEL_ConfigPin(&PinCfg);


	ADC_Init(LPC_ADC, 200000);

	ADC_IntConfig(LPC_ADC,ADC_ADINTEN0,DISABLE);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN1,DISABLE);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN2,DISABLE);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN3,DISABLE);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN4,DISABLE);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN5,DISABLE);
}


uint16_t get_adc_value_channel(uint8_t channel)
{
	uint16_t val = 0xF000;
	if(channel == 0)
	{
		ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_0,ENABLE);
		ADC_StartCmd(LPC_ADC,ADC_START_NOW);
		while(!(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_0,ADC_DATA_DONE)));
		val= ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_0);
		ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_0,DISABLE);
		val&=0x0FFF;
		return val;
	}
	else if(channel == 1)
	{
		ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_1,ENABLE);
		ADC_StartCmd(LPC_ADC,ADC_START_NOW);
		while(!(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_1,ADC_DATA_DONE)));
		val= ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_1);
		ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_1,DISABLE);
		val&=0x0FFF;
		return val;
	}
	else if(channel == 2)
	{
		ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_2,ENABLE);
		ADC_StartCmd(LPC_ADC,ADC_START_NOW);
		while(!(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_2,ADC_DATA_DONE)));
		val= ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_2);
		ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_2,DISABLE);
		val&=0x0FFF;
		return val;
	}
	else if(channel == 3)
	{
		ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_3,ENABLE);
		ADC_StartCmd(LPC_ADC,ADC_START_NOW);
		while(!(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_3,ADC_DATA_DONE)));
		val= ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_3);
		ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_3,DISABLE);
		val&=0x0FFF;
		return val;
	}
	else if(channel == 4)
	{
		ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_4,ENABLE);
		ADC_StartCmd(LPC_ADC,ADC_START_NOW);
		while(!(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_4,ADC_DATA_DONE)));
		val= ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_4);
		ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_4,DISABLE);
		val&=0x0FFF;
		return val;
	}
	else if(channel == 5)
	{
		ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_5,ENABLE);
		ADC_StartCmd(LPC_ADC,ADC_START_NOW);
		while(!(ADC_ChannelGetStatus(LPC_ADC,ADC_CHANNEL_5,ADC_DATA_DONE)));
		val= ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_5);
		ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_5,DISABLE);
		val&=0x0FFF;
		return val;
	}
	return 0;
}
