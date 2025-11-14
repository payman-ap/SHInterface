/******************************************************************
 *****                                                        *****
 *****  Name: encoder_read.c                                  *****
 *****  Ver.: 1.0                                             *****
 *****  Date: 28/09/2016                                      *****
 *****  Auth: Payman Azaripasand                              *****
 *****  Func: implements a dynamic Software-Hardware          *****
 *****        Interface to be used in Matlab Simulink         *****
 *****                                                        *****
 ******************************************************************/

#include "encoder_read.h"


extern uint8_t QENC_PORTS[];
extern uint8_t QENC_PINS[];
extern uint8_t QENC_TIMERS[];
extern uint8_t QENC_CAPS[];

//Private Variables
uint32_t tempMatchCount[CAP_CHANNEL_MAX_INDEX+1];
uint32_t MatchCount[CAP_CHANNEL_MAX_INDEX+1];
int32_t PulseCount[CAP_CHANNEL_MAX_INDEX+1];
uint8_t PinState[CAP_CHANNEL_MAX_INDEX+1];
_Bool isIntOnRising[CAP_CHANNEL_MAX_INDEX+1];
_Bool isMatchValueValid[CAP_CHANNEL_MAX_INDEX+1];

TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct;
TIM_CAPTURECFG_Type TIM_CaptureConfigStruct;
int32_t PulseCounter[3];
_Bool isQenc1Initialized;
_Bool isQenc3Initialized;
_Bool isQenc5Initialized;
uint32_t IRVAL1;
uint32_t IRVAL3;
uint32_t IRVAL5;


void init_qencoder()
{

	uint8_t i;
	PulseCounter[0] = 0;
	PulseCounter[1] = 0;
	PulseCounter[2] = 0;
	isQenc1Initialized=0;
	isQenc3Initialized=0;
	isQenc5Initialized=0;

	for (i=CAP_CHANNEL_MIN_INDEX; i<=CAP_CHANNEL_MAX_INDEX; i++)
	{
		isIntOnRising[i]=1;
		MatchCount[i]=0;
		isMatchValueValid[i]=0;
	}
	qenc_channel_begin(1);
	//qenc_channel_begin(2);
	qenc_channel_begin(3);
//	qenc_channel_begin(4);
//	qenc_channel_begin(5);
	qenc_channel_begin(6);


	IRVAL1=99;
	IRVAL3=99;
	IRVAL5=99;
}

void qenc_channel_begin(uint8_t channel)
{

	PINSEL_CFG_Type PinCfg;

	if(channel<CAP_CHANNEL_MIN_INDEX || channel>CAP_CHANNEL_MAX_INDEX)
		return;

	PinCfg.Funcnum = 3;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = QENC_PORTS[channel];
	PinCfg.Pinnum = QENC_PINS[channel];
	PINSEL_ConfigPin(&PinCfg);

	// Initialize timer, prescale count time of 1us
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 1;

	// use channel
	TIM_CaptureConfigStruct.CaptureChannel = QENC_CAPS[channel];
	// Enable capture on rising edge
	TIM_CaptureConfigStruct.RisingEdge = ENABLE;
	// Disable capture on falling edge
	TIM_CaptureConfigStruct.FallingEdge = DISABLE;
	// Generate capture interrupt
	TIM_CaptureConfigStruct.IntOnCaption = ENABLE;
	isIntOnRising[channel]=1;

	switch (channel)
	{
	case 1:
	case 2:

		if(!isQenc1Initialized)
		{
			TIM_Init(LPC_TIM(QENC1_TIM), TIM_TIMER_MODE,&TIM_ConfigStruct);
		}

		TIM_ConfigCapture(LPC_TIM(QENC1_TIM), &TIM_CaptureConfigStruct);
		TIM_ResetCounter(LPC_TIM(QENC1_TIM));

		if(!isQenc1Initialized)
		{
			/* preemption = 1, sub-priority = 1 */
			//TODO check priority
			NVIC_SetPriority(TIMER_IRQn(QENC1_TIM), ((0x01<<4)|0x01));

			/* Enable interrupt for timer */
			NVIC_EnableIRQ(TIMER_IRQn(QENC1_TIM));
			// To start timer
			TIM_Cmd(LPC_TIM(QENC1_TIM),ENABLE);
			isQenc1Initialized=1;
		}
		break;

	case 3:
	case 4:
		if(!isQenc3Initialized)
		{
			TIM_Init(LPC_TIM(QENC3_TIM), TIM_TIMER_MODE,&TIM_ConfigStruct);
		}

		TIM_ConfigCapture(LPC_TIM(QENC3_TIM), &TIM_CaptureConfigStruct);
		TIM_ResetCounter(LPC_TIM(QENC3_TIM));

		if(!isQenc3Initialized)
		{
			//TODO check priority
			NVIC_SetPriority(TIMER_IRQn(QENC3_TIM), ((0x01<<4)|0x01));
			NVIC_EnableIRQ(TIMER_IRQn(QENC3_TIM));
			TIM_Cmd(LPC_TIM(QENC3_TIM),ENABLE);
			isQenc3Initialized=1;
		}
		break;
	case 5:
	case 6:
		if(!isQenc5Initialized)
		{
			TIM_Init(LPC_TIM(QENC5_TIM), TIM_TIMER_MODE,&TIM_ConfigStruct);
		}

		TIM_ConfigCapture(LPC_TIM(QENC5_TIM), &TIM_CaptureConfigStruct);
		TIM_ResetCounter(LPC_TIM(QENC5_TIM));

		if(!isQenc5Initialized)
		{
			//TODO check priority
			NVIC_SetPriority(TIMER_IRQn(QENC5_TIM), ((0x01<<4)|0x01));
			NVIC_EnableIRQ(TIMER_IRQn(QENC5_TIM));
			TIM_Cmd(LPC_TIM(QENC5_TIM),ENABLE);
			isQenc5Initialized=1;
		}
		break;
	}

}

short get_qenc_value(uint8_t channel)
{
	return PulseCounter[channel];
}

void reset_qencoder(uint8_t channel){
	PulseCounter[channel] = 0;
}


void TIMER_IRQHandler(QENC1_TIM)
{
	uint8_t channel;
	//IRVAL1=(uint32_t)(LPC_TIM(QENC1_TIM)->IR);
	//LPC_TIM(QENC1_TIM)->IR |= (1<<(4) || (1<<5));
	LPC_TIM(QENC1_TIM)->IR |= (1<<(4+QENC_CAPS[1])) | (1<<(4+QENC_CAPS[2]));

	if((((LPC_GPIO0->FIOPIN)&(1<<4))>>4)==(((LPC_GPIO0->FIOPIN)&(1<<5))>>5)){
		PulseCounter[2]=PulseCounter[2]-1;
	}
	else{
		PulseCounter[2]=PulseCounter[2]+1;
	}
	/*if(IRVAL1 == 48)
	{
		for(channel=1;channel<=2;channel++)
		{
			TIM_CaptureConfigStruct.CaptureChannel = QENC_CAPS[channel];

			if(isIntOnRising[channel])
			{
				PinState[channel]=1;

				//next interrupt is on falling edge
				TIM_CaptureConfigStruct.RisingEdge = DISABLE;
				TIM_CaptureConfigStruct.FallingEdge = ENABLE;

				tempMatchCount[channel]=TIM_GetCaptureValue(LPC_TIM(QENC1_TIM),QENC_CAPS[channel]);
				isMatchValueValid[channel]=0;

				isIntOnRising[channel]=0;

				TIM_ConfigCapture(LPC_TIM(QENC1_TIM), &TIM_CaptureConfigStruct);
			}
			else if(!isIntOnRising[channel])
			{
				PinState[channel]=0;

				MatchCount[channel]=TIM_GetCaptureValue(LPC_TIM(QENC1_TIM),QENC_CAPS[channel])-tempMatchCount[channel];
				isMatchValueValid[channel]=1;

				//next interrupt is on rising edge
				TIM_CaptureConfigStruct.RisingEdge = ENABLE;
				TIM_CaptureConfigStruct.FallingEdge = DISABLE;
				isIntOnRising[channel]=1;

				TIM_ConfigCapture(LPC_TIM(QENC1_TIM), &TIM_CaptureConfigStruct);
			}
		}

		LPC_TIM(QENC1_TIM)->IR |=  ((((uint32_t)1)<<(4+QENC_CAPS[1])) | (((uint32_t)1)<<(4+QENC_CAPS[2])));

		if((isMatchValueValid[1] && isMatchValueValid[2]))
		{
			TIM_ResetCounter(LPC_TIM(QENC1_TIM));
		}
	}

	else if(IRVAL1 == 32)
	{
		channel = 2;
		TIM_CaptureConfigStruct.CaptureChannel = QENC_CAPS[channel];

		if(isIntOnRising[channel])
		{
			PinState[channel]=1;

			if(PinState[channel-1]==PinState[channel])
			{
				PulseCount[channel]++;
			}
			else
			{
				PulseCount[channel]--;
			}
			//next interrupt is on falling edge
			TIM_CaptureConfigStruct.RisingEdge = DISABLE;
			TIM_CaptureConfigStruct.FallingEdge = ENABLE;

			tempMatchCount[channel]=TIM_GetCaptureValue(LPC_TIM(QENC1_TIM),QENC_CAPS[channel]);
			isMatchValueValid[channel]=0;

			isIntOnRising[channel]=0;

			TIM_ConfigCapture(LPC_TIM(QENC1_TIM), &TIM_CaptureConfigStruct);
		}
		else
		{
			PinState[channel]=0;

//			if(PinState[channel-1]==PinState[channel])
//			{
//				PulseCount[channel]++;
//			}
//			else
//			{
//				PulseCount[channel]--;
//			}
			MatchCount[channel]=TIM_GetCaptureValue(LPC_TIM(QENC1_TIM),QENC_CAPS[channel])-tempMatchCount[channel];
			isMatchValueValid[channel]=1;

			//next interrupt is on rising edge
			TIM_CaptureConfigStruct.RisingEdge = ENABLE;
			TIM_CaptureConfigStruct.FallingEdge = DISABLE;
			isIntOnRising[channel]=1;

			TIM_ConfigCapture(LPC_TIM(QENC1_TIM), &TIM_CaptureConfigStruct);
		}
		LPC_TIM(QENC1_TIM)->IR |=  (((uint32_t)1)<<(4+QENC_CAPS[channel]));

		if((isMatchValueValid[1] && isMatchValueValid[2]))
		{
			TIM_ResetCounter(LPC_TIM(QENC3_TIM));
		}
	}
	else if(IRVAL1 == 16)
	{
		channel = 1;
		TIM_CaptureConfigStruct.CaptureChannel = QENC_CAPS[channel];

		if(isIntOnRising[channel])
		{
			PinState[channel]=1;

		if(PinState[channel+1]==PinState[channel])
		{
			PulseCount[channel]++;
		}
		else
		{
			PulseCount[channel]--;
		}
			//next interrupt is on falling edge
			TIM_CaptureConfigStruct.RisingEdge = DISABLE;
			TIM_CaptureConfigStruct.FallingEdge = ENABLE;

			tempMatchCount[channel]=TIM_GetCaptureValue(LPC_TIM(QENC1_TIM),QENC_CAPS[channel]);
			isMatchValueValid[channel]=0;

			isIntOnRising[channel]=0;

			TIM_ConfigCapture(LPC_TIM(QENC1_TIM), &TIM_CaptureConfigStruct);
		}
		else
		{
			PinState[channel]=0;

//			if(PinState[channel+1]==PinState[channel])
//			{
//				PulseCount[channel]++;
//			}
//			else
//			{
//				PulseCount[channel]--;
//			}
			MatchCount[channel]=TIM_GetCaptureValue(LPC_TIM(QENC1_TIM),QENC_CAPS[channel])-tempMatchCount[channel];
			isMatchValueValid[channel]=1;

			//next interrupt is on rising edge
			TIM_CaptureConfigStruct.RisingEdge = ENABLE;
			TIM_CaptureConfigStruct.FallingEdge = DISABLE;
			isIntOnRising[channel]=1;

			TIM_ConfigCapture(LPC_TIM(QENC1_TIM), &TIM_CaptureConfigStruct);
		}
		LPC_TIM(QENC1_TIM)->IR |=  (((uint32_t)1)<<(4+QENC_CAPS[channel]));

		if((isMatchValueValid[1] && isMatchValueValid[2]))
		{
			TIM_ResetCounter(LPC_TIM(QENC3_TIM));
		}

	}

	else
	{
		LPC_TIM(QENC1_TIM)->IR |= (1<<(4+QENC_CAPS[1])) | (1<<(4+QENC_CAPS[2]));
	}*/
}

void TIMER_IRQHandler(QENC3_TIM)
{
	uint8_t channel;
	//IRVAL3=(uint32_t)(LPC_TIM(QENC3_TIM)->IR);
	
	//LPC_TIM(QENC3_TIM)->IR |= (1<<(4) || (1<<5));
	LPC_TIM(QENC3_TIM)->IR |= (1<<(4+QENC_CAPS[3])) | (1<<(4+QENC_CAPS[4]));
	if((((LPC_GPIO1->FIOPIN)&(1<<18))>>18)==(((LPC_GPIO1->FIOPIN)&(1<<19))>>19)){
		PulseCounter[1]=PulseCounter[1]-1;
	}
	else{
		PulseCounter[1]=PulseCounter[1]+1;
	}
	
	/*
	if(IRVAL3 == 48)
	{
		for(channel=3;channel<=4;channel++)
		{
			TIM_CaptureConfigStruct.CaptureChannel = QENC_CAPS[channel];

			if(isIntOnRising[channel])
			{
				PinState[channel]=1;

				//next interrupt is on falling edge
				TIM_CaptureConfigStruct.RisingEdge = DISABLE;
				TIM_CaptureConfigStruct.FallingEdge = ENABLE;

				tempMatchCount[channel]=TIM_GetCaptureValue(LPC_TIM(QENC3_TIM),QENC_CAPS[channel]);
				isMatchValueValid[channel]=0;

				isIntOnRising[channel]=0;

				TIM_ConfigCapture(LPC_TIM(QENC3_TIM), &TIM_CaptureConfigStruct);
			}
			else if(!isIntOnRising[channel])
			{
				PinState[channel]=0;

				MatchCount[channel]=TIM_GetCaptureValue(LPC_TIM(QENC3_TIM),QENC_CAPS[channel])-tempMatchCount[channel];
				isMatchValueValid[channel]=1;

				//next interrupt is on rising edge
				TIM_CaptureConfigStruct.RisingEdge = ENABLE;
				TIM_CaptureConfigStruct.FallingEdge = DISABLE;
				isIntOnRising[channel]=1;

				TIM_ConfigCapture(LPC_TIM(QENC3_TIM), &TIM_CaptureConfigStruct);
			}
		}

		LPC_TIM(QENC3_TIM)->IR |=  ((((uint32_t)1)<<(4+QENC_CAPS[3])) | (((uint32_t)1)<<(4+QENC_CAPS[4])));

		if((isMatchValueValid[3] && isMatchValueValid[4]))
		{
			TIM_ResetCounter(LPC_TIM(QENC3_TIM));
		}
	}

	else if(IRVAL3 == 32)
	{
		channel = 4;
		TIM_CaptureConfigStruct.CaptureChannel = QENC_CAPS[channel];

		if(isIntOnRising[channel])
		{
			PinState[channel]=1;

			if(PinState[channel-1]==PinState[channel])
			{
				PulseCount[channel]++;
			}
			else
			{
				PulseCount[channel]--;
			}
			//next interrupt is on falling edge
			TIM_CaptureConfigStruct.RisingEdge = DISABLE;
			TIM_CaptureConfigStruct.FallingEdge = ENABLE;

			tempMatchCount[channel]=TIM_GetCaptureValue(LPC_TIM(QENC3_TIM),QENC_CAPS[channel]);
			isMatchValueValid[channel]=0;

			isIntOnRising[channel]=0;

			TIM_ConfigCapture(LPC_TIM(QENC3_TIM), &TIM_CaptureConfigStruct);
		}
		else
		{
			PinState[channel]=0;

//			if(PinState[channel-1]==PinState[channel])
//			{
//				PulseCount[channel]++;
//			}
//			else
//			{
//				PulseCount[channel]--;
//			}
			MatchCount[channel]=TIM_GetCaptureValue(LPC_TIM(QENC3_TIM),QENC_CAPS[channel])-tempMatchCount[channel];
			isMatchValueValid[channel]=1;

			//next interrupt is on rising edge
			TIM_CaptureConfigStruct.RisingEdge = ENABLE;
			TIM_CaptureConfigStruct.FallingEdge = DISABLE;
			isIntOnRising[channel]=1;

			TIM_ConfigCapture(LPC_TIM(QENC3_TIM), &TIM_CaptureConfigStruct);
		}
		LPC_TIM(QENC3_TIM)->IR |=  (((uint32_t)1)<<(4+QENC_CAPS[channel]));

		if((isMatchValueValid[3] && isMatchValueValid[4]))
		{
			TIM_ResetCounter(LPC_TIM(QENC3_TIM));
		}

	}
	else if(IRVAL3 == 16)
	{
		channel = 3;
		TIM_CaptureConfigStruct.CaptureChannel = QENC_CAPS[channel];

		if(isIntOnRising[channel])
		{
			PinState[channel]=1;

			if(PinState[channel+1]==PinState[channel])
			{
				PulseCount[channel]++;
			}
			else
			{
				PulseCount[channel]--;
			}
			//next interrupt is on falling edge
			TIM_CaptureConfigStruct.RisingEdge = DISABLE;
			TIM_CaptureConfigStruct.FallingEdge = ENABLE;

			tempMatchCount[channel]=TIM_GetCaptureValue(LPC_TIM(QENC3_TIM),QENC_CAPS[channel]);
			isMatchValueValid[channel]=0;

			isIntOnRising[channel]=0;

			TIM_ConfigCapture(LPC_TIM(QENC3_TIM), &TIM_CaptureConfigStruct);
		}
		else
		{
			PinState[channel]=0;

//			if(PinState[channel+1]==PinState[channel])
//			{
//				PulseCount[channel]++;
//			}
//			else
//			{
//				PulseCount[channel]--;
//			}
			MatchCount[channel]=TIM_GetCaptureValue(LPC_TIM(QENC3_TIM),QENC_CAPS[channel])-tempMatchCount[channel];
			isMatchValueValid[channel]=1;

			//next interrupt is on rising edge
			TIM_CaptureConfigStruct.RisingEdge = ENABLE;
			TIM_CaptureConfigStruct.FallingEdge = DISABLE;
			isIntOnRising[channel]=1;

			TIM_ConfigCapture(LPC_TIM(QENC3_TIM), &TIM_CaptureConfigStruct);
		}
		LPC_TIM(QENC3_TIM)->IR |=  (((uint32_t)1)<<(4+QENC_CAPS[channel]));

		if((isMatchValueValid[3] && isMatchValueValid[4]))
		{
			TIM_ResetCounter(LPC_TIM(QENC3_TIM));
		}

	}

	else
	{
		LPC_TIM(QENC3_TIM)->IR |= (1<<(4+QENC_CAPS[3])) | (1<<(4+QENC_CAPS[4]));
	}
	*/
}

void TIMER_IRQHandler(QENC5_TIM)
{
	uint8_t channel;
	//IRVAL5=(uint32_t)(LPC_TIM(QENC5_TIM)->IR);
	
	//LPC_TIM(QENC3_TIM)->IR |= (1<<(4) || (1<<5));
	LPC_TIM(QENC5_TIM)->IR |= (1<<(4+QENC_CAPS[5])) | (1<<(4+QENC_CAPS[6]));
	if((((LPC_GPIO1->FIOPIN)&(1<<26))>>26)==(((LPC_GPIO1->FIOPIN)&(1<<27))>>27)){
		PulseCounter[0]=PulseCounter[0]-1;
	}
	else{
		PulseCounter[0]=PulseCounter[0]+1;
	}
	
	/*
	if(IRVAL5 == 48)
	{
		for(channel=5;channel<=6;channel++)
		{
			TIM_CaptureConfigStruct.CaptureChannel = QENC_CAPS[channel];

			if(isIntOnRising[channel])
			{
				PinState[channel]=1;

				//next interrupt is on falling edge
				TIM_CaptureConfigStruct.RisingEdge = DISABLE;
				TIM_CaptureConfigStruct.FallingEdge = ENABLE;

				tempMatchCount[channel]=TIM_GetCaptureValue(LPC_TIM(QENC5_TIM),QENC_CAPS[channel]);
				isMatchValueValid[channel]=0;

				isIntOnRising[channel]=0;

				TIM_ConfigCapture(LPC_TIM(QENC5_TIM), &TIM_CaptureConfigStruct);
			}
			else if(!isIntOnRising[channel])
			{
				PinState[channel]=0;

				MatchCount[channel]=TIM_GetCaptureValue(LPC_TIM(QENC5_TIM),QENC_CAPS[channel])-tempMatchCount[channel];
				isMatchValueValid[channel]=1;

				//next interrupt is on rising edge
				TIM_CaptureConfigStruct.RisingEdge = ENABLE;
				TIM_CaptureConfigStruct.FallingEdge = DISABLE;
				isIntOnRising[channel]=1;

				TIM_ConfigCapture(LPC_TIM(QENC5_TIM), &TIM_CaptureConfigStruct);
			}
		}

		LPC_TIM(QENC5_TIM)->IR |=  ((((uint32_t)1)<<(4+QENC_CAPS[5])) | (((uint32_t)1)<<(4+QENC_CAPS[6])));

		if((isMatchValueValid[5] && isMatchValueValid[6]))
		{
			TIM_ResetCounter(LPC_TIM(QENC5_TIM));
		}
	}

	else if(IRVAL5 == 32)
	{
		channel = 6;
		TIM_CaptureConfigStruct.CaptureChannel = QENC_CAPS[channel];

		if(isIntOnRising[channel])
		{
			PinState[channel]=1;

			if(PinState[channel-1]==PinState[channel])
			{
				PulseCount[channel]++;
			}
			else
			{
				PulseCount[channel]--;
			}
			//next interrupt is on falling edge
			TIM_CaptureConfigStruct.RisingEdge = DISABLE;
			TIM_CaptureConfigStruct.FallingEdge = ENABLE;

			tempMatchCount[channel]=TIM_GetCaptureValue(LPC_TIM(QENC5_TIM),QENC_CAPS[channel]);
			isMatchValueValid[channel]=0;

			isIntOnRising[channel]=0;

			TIM_ConfigCapture(LPC_TIM(QENC5_TIM), &TIM_CaptureConfigStruct);
		}
		else
		{
			PinState[channel]=0;

//			if(PinState[channel-1]==PinState[channel])
//			{
//				PulseCount[channel]++;
//			}
//			else
//			{
//				PulseCount[channel]--;
//			}
			MatchCount[channel]=TIM_GetCaptureValue(LPC_TIM(QENC5_TIM),QENC_CAPS[channel])-tempMatchCount[channel];
			isMatchValueValid[channel]=1;

			//next interrupt is on rising edge
			TIM_CaptureConfigStruct.RisingEdge = ENABLE;
			TIM_CaptureConfigStruct.FallingEdge = DISABLE;
			isIntOnRising[channel]=1;

			TIM_ConfigCapture(LPC_TIM(QENC5_TIM), &TIM_CaptureConfigStruct);
		}
		LPC_TIM(QENC5_TIM)->IR |=  (((uint32_t)1)<<(4+QENC_CAPS[channel]));

		if((isMatchValueValid[5] && isMatchValueValid[6]))
		{
			TIM_ResetCounter(LPC_TIM(QENC5_TIM));
		}

	}
	else if(IRVAL5 == 16)
	{
		channel = 5;
		TIM_CaptureConfigStruct.CaptureChannel = QENC_CAPS[channel];

		if(isIntOnRising[channel])
		{
			PinState[channel]=1;
			if(PinState[channel+1]==PinState[channel])
			{
				PulseCount[channel]++;
			}
			else
			{
				PulseCount[channel]--;
			}
			//next interrupt is on falling edge
			TIM_CaptureConfigStruct.RisingEdge = DISABLE;
			TIM_CaptureConfigStruct.FallingEdge = ENABLE;

			tempMatchCount[channel]=TIM_GetCaptureValue(LPC_TIM(QENC5_TIM),QENC_CAPS[channel]);
			isMatchValueValid[channel]=0;
			isIntOnRising[channel]=0;

			TIM_ConfigCapture(LPC_TIM(QENC5_TIM), &TIM_CaptureConfigStruct);
		}
		else
		{
			PinState[channel]=0;
//			if(PinState[channel+1]==PinState[channel])
//			{
//				PulseCount[channel]++;
//			}
//			else
//			{
//				PulseCount[channel]--;
//			}
			MatchCount[channel]=TIM_GetCaptureValue(LPC_TIM(QENC5_TIM),QENC_CAPS[channel])-tempMatchCount[channel];
			isMatchValueValid[channel]=1;

			//next interrupt is on rising edge
			TIM_CaptureConfigStruct.RisingEdge = ENABLE;
			TIM_CaptureConfigStruct.FallingEdge = DISABLE;
			isIntOnRising[channel]=1;

			TIM_ConfigCapture(LPC_TIM(QENC5_TIM), &TIM_CaptureConfigStruct);
		}
		LPC_TIM(QENC5_TIM)->IR |=  (((uint32_t)1)<<(4+QENC_CAPS[channel]));

		if((isMatchValueValid[5] && isMatchValueValid[6]))
		{
			TIM_ResetCounter(LPC_TIM(QENC5_TIM));
		}
	}

	else
	{
		LPC_TIM(QENC5_TIM)->IR |= (1<<(4+QENC_CAPS[5])) | (1<<(4+QENC_CAPS[6]));
	}
	*/
}
