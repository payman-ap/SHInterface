/******************************************************************
 *****                                                        *****
 *****  Name: pwm_write.h                                     *****
 *****  Ver.: 1.0                                             *****
 *****  Date: 28/09/2016                                      *****
 *****  Auth: Payman Azaripasand                              *****
 *****  Func: implements a dynamic Software-Hardware          *****
 *****        Interface to be used in Matlab Simulink         *****
 *****                                                        *****
 ******************************************************************/

#ifndef PWM_H
#define PWM_H

#include "initparams.h"

#include "lpc_types.h"

#define MOTOR_CHANNEL 1
#define MOTOR_CHANNEL_WIDTH 2

#define SERVO_CHANNEL 3
#define SERVO_CHANNEL_WIDTH 4

void init_pwm(void);
void set_motor_pw(uint8_t channel_number, uint16_t pw);
void set_servo_pw(uint16_t channel_number, uint16_t pw);
void enable_interrupt(void);
void PWM1_IRQHandler(void);

#endif //PWM_H
