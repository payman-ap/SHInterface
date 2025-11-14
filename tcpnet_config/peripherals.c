#include "peripherals.h"
#include "digit_out.h"
#include "encoder_read.h"
#include "pwm_write.h"
#include "usb.h"
#include "analog_to_digit_conv.h"

void conf_peris(peripheral_conf params)
{
	init_qencoder();
	
	if(params.adc_flag)
	{
		init_adc();
	}
	if(params.pwm_flag)
	{
		init_pwm();
	}
	if(params.usb_flag)
	{
		init_usb(params.usb_config);
	}
	if(params.dio_flag)
	{
		init_DIO();
	}
}
