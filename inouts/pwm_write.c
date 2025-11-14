/******************************************************************
 *****                                                        *****
 *****  Name: pwm_write.c                                     *****
 *****  Ver.: 1.0                                             *****
 *****  Date: 28/09/2016                                      *****
 *****  Auth: Payman Azaripasand                              *****
 *****  Func: implements a dynamic Software-Hardware          *****
 *****        Interface to be used in Matlab Simulink         *****
 *****                                                        *****
 ******************************************************************/

#include "pwm_write.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_pwm.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_clkpwr.h"

uint8_t motor_pw[MOTOR_CHANNEL_WIDTH];
uint16_t servo_pw[SERVO_CHANNEL_WIDTH];
uint8_t channels[SERVO_CHANNEL_WIDTH];
uint16_t servo_pulse_counter = 0;

void init_pwm()
{
	PINSEL_CFG_Type PinCfg;
	PWM_TIMERCFG_Type PWMCfgDat;
	PWM_MATCHCFG_Type PWMMatchCfgDat;

	unsigned long motor2_dir0  =  ((uint32_t)1<<8);
	unsigned long motor2_dir1  =  ((uint32_t)1<<9);
	unsigned long motor1_dir0  =  ((uint32_t)1<<6);
	unsigned long motor1_dir1  =  ((uint32_t)1<<7);

	//Direction Pin init
	PinCfg.Funcnum = M1D_FCN;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = M1D_PRT;
	PinCfg.Pinnum  = M1D0_PIN;
	PINSEL_ConfigPin(&PinCfg);

	PinCfg.Pinnum  = M1D1_PIN;
	PINSEL_ConfigPin(&PinCfg);

	PinCfg.Funcnum = M2D_FCN;
	PinCfg.Portnum = M2D_PRT;
	PinCfg.Pinnum  = M2D0_PIN;
	PINSEL_ConfigPin(&PinCfg);

	PinCfg.Pinnum  = M2D1_PIN;
	PINSEL_ConfigPin(&PinCfg);
	//End of Direction Pin init


	GPIO_SetDir (2, motor1_dir0, 1);
	GPIO_SetDir (2, motor1_dir1, 1);
	GPIO_SetValue	(2, motor1_dir0);
	GPIO_ClearValue	(2, motor1_dir1);


	GPIO_SetDir (2, motor2_dir0, 1);
	GPIO_SetDir (2, motor2_dir1, 1);
	GPIO_SetValue	(2, motor2_dir0);
	GPIO_ClearValue	(2, motor2_dir1);


	PinCfg.Funcnum = PWM0_FCN;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = PWM0_PRT;
	PinCfg.Pinnum  = PWM0_PIN;
	PINSEL_ConfigPin(&PinCfg);

	PinCfg.Funcnum = PWM1_FCN;
	PinCfg.Portnum = PWM1_PRT;
	PinCfg.Pinnum  = PWM1_PIN;
	PINSEL_ConfigPin(&PinCfg);

	PinCfg.Funcnum = SERVO1_FCN;
	PinCfg.Portnum = SERVO1_PRT;
	PinCfg.Pinnum  = SERVO1_PIN;
	PINSEL_ConfigPin(&PinCfg);

	PinCfg.Funcnum = SERVO2_FCN;
	PinCfg.Portnum = SERVO2_PRT;
	PinCfg.Pinnum  = SERVO2_PIN;
	PINSEL_ConfigPin(&PinCfg);

	PinCfg.Funcnum = SERVO3_FCN;
	PinCfg.Portnum = SERVO3_PRT;
	PinCfg.Pinnum  = SERVO3_PIN;
	PINSEL_ConfigPin(&PinCfg);

	PinCfg.Funcnum = SERVO4_FCN;
	PinCfg.Portnum = SERVO4_PRT;
	PinCfg.Pinnum  = SERVO4_PIN;
	PINSEL_ConfigPin(&PinCfg);

	PWMCfgDat.PrescaleOption = PWM_TIMER_PRESCALE_USVAL;
	PWMCfgDat.PrescaleValue = 1;
	PWM_Init(LPC_PWM1, PWM_MODE_TIMER, (void *) &PWMCfgDat);
	PWMMatchCfgDat.IntOnMatch = DISABLE;
	PWMMatchCfgDat.MatchChannel = 0;
	PWMMatchCfgDat.ResetOnMatch = ENABLE;
	PWMMatchCfgDat.StopOnMatch = DISABLE;
	PWM_ConfigMatch(LPC_PWM1, &PWMMatchCfgDat);

	PWM_ChannelConfig(LPC_PWM1, 2, PWM_CHANNEL_SINGLE_EDGE);
	PWM_ChannelConfig(LPC_PWM1, 3, PWM_CHANNEL_SINGLE_EDGE);
	PWM_ChannelConfig(LPC_PWM1, 4, PWM_CHANNEL_SINGLE_EDGE);
	PWM_ChannelConfig(LPC_PWM1, 5, PWM_CHANNEL_SINGLE_EDGE);
	PWM_ChannelConfig(LPC_PWM1, 6, PWM_CHANNEL_SINGLE_EDGE);

	PWM_CounterCmd(LPC_PWM1, ENABLE);
	PWM_MatchUpdate(LPC_PWM1, 0, 100, PWM_MATCH_UPDATE_NOW);

	PWM_MatchUpdate(LPC_PWM1, MOTOR1, 0, PWM_MATCH_UPDATE_NOW);
	PWM_MatchUpdate(LPC_PWM1, MOTOR2, 0, PWM_MATCH_UPDATE_NOW);
	PWM_MatchUpdate(LPC_PWM1, SERVO1, 0, PWM_MATCH_UPDATE_NOW);
	PWM_MatchUpdate(LPC_PWM1, SERVO2, 0, PWM_MATCH_UPDATE_NOW);
	PWM_MatchUpdate(LPC_PWM1, SERVO3, 0, PWM_MATCH_UPDATE_NOW);
	PWM_MatchUpdate(LPC_PWM1, SERVO4, 0, PWM_MATCH_UPDATE_NOW);

	PWM_Cmd(LPC_PWM1, ENABLE);

	PWM_ChannelCmd(LPC_PWM1, MOTOR1, ENABLE);
	PWM_ChannelCmd(LPC_PWM1, MOTOR2, ENABLE);
	PWM_ChannelCmd(LPC_PWM1, SERVO1, ENABLE);
	PWM_ChannelCmd(LPC_PWM1, SERVO2, ENABLE);
	PWM_ChannelCmd(LPC_PWM1, SERVO3, ENABLE);
	PWM_ChannelCmd(LPC_PWM1, SERVO4, ENABLE);

	servo_pw[0] = MAX_SERVO;
	servo_pw[1] = MAX_SERVO;
	servo_pw[2] = MAX_SERVO;
	servo_pw[3] = MAX_SERVO;
	
	channels[0] = SERVO1;
	channels[1] = SERVO2;
	channels[2] = SERVO3;
	channels[3] = SERVO4;
	
	enable_interrupt();
}

void set_motor_pw(uint8_t channel_number, uint16_t pw)
{
	uint8_t pwValue;
	uint8_t direction;
	if(pw > 127)
	{
		pwValue = 255 - pw;
		direction = 1;
	}
	else
	{
		pwValue = pw;
		direction = 0;
	}
	motor_pw[channel_number] = pwValue;
	PWM_MatchUpdate(LPC_PWM1, channel_number + 5, pwValue, PWM_MATCH_UPDATE_NEXT_RST);
	if(channel_number == 0)
	{
		if(direction == 0)
		{
			GPIO_ClearValue(M1D_PRT, 1<<M1D1_PIN);
			GPIO_SetValue(M1D_PRT, 1<<M1D0_PIN);
		}
		else
		{
			GPIO_ClearValue(M1D_PRT, 1<<M1D0_PIN);
			GPIO_SetValue(M1D_PRT, 1<<M1D1_PIN);
		}
	}
	else
	{
		if(direction == 0)
		{
			GPIO_ClearValue(M2D_PRT, 1<<M2D1_PIN);
			GPIO_SetValue(M2D_PRT, 1<<M2D0_PIN);
		}
		else
		{
			GPIO_ClearValue(M2D_PRT, 1<<M2D0_PIN);
			GPIO_SetValue(M2D_PRT, 1<<M2D1_PIN);
		}
	}
}

void set_servo_pw(uint16_t channel_number, uint16_t pw)
{
	uint8_t index = 0;
	uint16_t temp;
	if(channels[0] == channel_number)
		index = 0;
	else if(channels[1] == channel_number)
		index = 1;
	else if(channels[2] == channel_number)
		index = 2;
	else if(channels[3] == channel_number)
		index = 3;

	servo_pw[index] = pw;

	if(servo_pw[0] > servo_pw[1])
	{
		temp = servo_pw[0];
		servo_pw[0] = servo_pw[1];
		servo_pw[1] = temp;
		temp = channels[0];
		channels[0] = channels[1];
		channels[1] = temp;
	}

	if(servo_pw[1] > servo_pw[2])
	{
		temp = servo_pw[1];
		servo_pw[1] = servo_pw[2];
		servo_pw[2] = temp;
		temp = channels[1];
		channels[1] = channels[2];
		channels[2] = temp;
	}

	if(servo_pw[2] > servo_pw[3])
	{
		temp = servo_pw[2];
		servo_pw[2] = servo_pw[3];
		servo_pw[3] = temp;
		temp = channels[2];
		channels[2] = channels[3];
		channels[3] = temp;
	}
}

void enable_interrupt()
{
	PWM_MATCHCFG_Type PWMMatchCfgDat;
	PWMMatchCfgDat.IntOnMatch = ENABLE;
	PWMMatchCfgDat.MatchChannel = 0;
	PWMMatchCfgDat.ResetOnMatch = ENABLE;
	PWMMatchCfgDat.StopOnMatch = DISABLE;
	PWM_ConfigMatch(LPC_PWM1, &PWMMatchCfgDat);

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(PWM1_IRQn, ((0x01<<3)|0x01));
	/* Enable Interrupt for PWM channel */
	NVIC_EnableIRQ(PWM1_IRQn);
}

void PWM1_IRQHandler()
{
	uint32_t IRt = LPC_PWM1->IR;
	uint8_t i;
	uint16_t s_pdiv100[4];
	s_pdiv100[0] = servo_pw[0] / 100;
	s_pdiv100[1] = servo_pw[1] / 100;
	s_pdiv100[2] = servo_pw[2] / 100;
	s_pdiv100[3] = servo_pw[3] / 100;
	if(IRt & 0x00000001)
	{
		for(i = 0; i < 4; i++)
		{
			if(servo_pulse_counter < s_pdiv100[i])
			{
				if(servo_pulse_counter == 0)
					PWM_MatchUpdate(LPC_PWM1, channels[i], 100, PWM_MATCH_UPDATE_NEXT_RST);
			}
			else if(servo_pulse_counter == s_pdiv100[i] && servo_pw[i] % 100 != 0)
			{
					PWM_Cmd(LPC_PWM1, DISABLE);
					PWM_MatchUpdate(LPC_PWM1, channels[i], servo_pw[i] % 100, PWM_MATCH_UPDATE_NEXT_RST);
					PWM_Cmd(LPC_PWM1, ENABLE);
			}
			else
			{
				if(servo_pulse_counter == s_pdiv100[i] || servo_pulse_counter == s_pdiv100[i] + 1)
					PWM_MatchUpdate(LPC_PWM1, channels[i], 0, PWM_MATCH_UPDATE_NEXT_RST);
			}
		}
		servo_pulse_counter++;
		if(servo_pulse_counter == 200)
			servo_pulse_counter = 0;
		LPC_PWM1->IR |= 0x00000001;
	}
	else
	{
		LPC_PWM1->IR = IRt | IRt;
	}
}
