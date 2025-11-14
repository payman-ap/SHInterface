#include "data_provider.h"

client_write_packet_struct client_write_param;
client_read_packet_struct client_read_param;
client_common_packet_struct client_common_param;

Bool length_problem, checksum_problem;

unsigned short update_client_param(unsigned char* packet)
{
	unsigned short i, index;
	unsigned char temp, PWM_state, Servo_state;
	unsigned short PWM_param_num = 0, servo_param_num = 0;
	//parse packet 0
	temp = packet[0];
	client_common_param.Digital_Input = temp % 4;
	client_common_param.ADC_Channel = temp >> 2;
	//parse packet 1
	temp = packet[1];
	client_common_param.Digital_Input *= 4;
	client_common_param.Digital_Input += temp >> 6;
	temp &= 0x3F;
	client_common_param.Encoder_Reset = temp >> 3;
	temp &= 0x07;
	client_common_param.Encoder_Read = temp;
	//parse packet 2
	temp = packet[2];
	temp &= 0x3F;
	client_common_param.PWM_state = PWM_state = temp >> 4;
	temp &= 0x0F;
	client_common_param.Servo_State = Servo_state = temp;
	//parse packet 3
	temp = packet[3];
	client_common_param.Digital_Output = temp;
	//PWM_param set
	index = 4;
	i = 0;
	while(PWM_state)
	{
		if(PWM_state % 2)
		{
			client_write_param.PWM_Param[i] = packet[index] << 8;
			client_write_param.PWM_Param[i] += packet[index + 1];
			index += 2;
			PWM_param_num++;
		}
		i++;
		PWM_state = PWM_state >> 1;
	}
	i = 0;
	while(Servo_state)
	{
		if(Servo_state % 2)
		{
			client_write_param.Servo_Param[i] = packet[index] << 8;
			client_write_param.Servo_Param[i] += packet[index + 1];
			index += 2;
			servo_param_num++;
		}
		i++;
		Servo_state = Servo_state >> 1;
	}
	if(client_common_param.Digital_Output != 0)
	{
		client_write_param.DO_Param = packet[index];
		index++;
	}
	return index;
}

unsigned short generate_reply_packet(unsigned char* packet)
{
	unsigned char length = 0, ADC_Channel, Enc_Read, i = 0, DI;
	unsigned char checksum = 0, channel = 0;
	ADC_Channel = client_common_param.ADC_Channel;
	Enc_Read = client_common_param.Encoder_Read;
	DI = client_common_param.Digital_Input;
	//header set
	packet[0] = 0xAA;
	packet[1] = 0xAB;

	//packet 3 set
	packet[3] = client_common_param.ADC_Channel;
	packet[3] = packet[3] << 2;
	packet[3] += client_common_param.Digital_Input >> 2;
	//packet 4 set
	packet[4] = client_common_param.Digital_Input % 4;
	packet[4] = packet[4] << 3;
	packet[4] += client_common_param.Encoder_Reset;
	packet[4] = packet[4] << 3;
	packet[4] += client_common_param.Encoder_Read;
	//packet 5 set
	packet[5] = client_common_param.PWM_state;
	packet[5] = packet[5] << 4;
	packet[5] += client_common_param.Servo_State;
	//packet 6 set
	packet[6] = client_common_param.Digital_Output;
	length = 7;
	while(ADC_Channel)
	{
		if(ADC_Channel % 2)
		{
			packet[length] = client_read_param.ADC_Param[i] >> 8;
			packet[length + 1] = client_read_param.ADC_Param[i] % 256;
			length += 2;
		}
		i++;
		ADC_Channel = ADC_Channel >> 1;
	}
	i = 0;
	if(DI)
	{
		packet[length] = 0;
		packet[length] = client_read_param.DI_Param;
		length++;
	}
	i = 0;
	while(Enc_Read)
	{
		if(Enc_Read % 2)
		{
			packet[length] = client_read_param.Enc_Read_Param[i] >> 8;
			packet[length + 1] = client_read_param.Enc_Read_Param[i] % 256;
			length += 2;
		}
		i++;
		Enc_Read = Enc_Read >> 1;
	}

	packet[2] = length - 2;

	for(i = 0; i < length; i++)
	{
		checksum = checksum ^ (unsigned char)packet[i];
	}
	packet[length] = checksum;
	length++;
	return length;
}

unsigned short generate_bad_request_packet(unsigned char* packet)
{
	if(length_problem)
	{
		packet[0] = 'e';
		packet[1] = 'r';
		packet[2] = 'r';
		packet[3] = 'o';
		packet[4] = 'r';
		packet[5] = 'l';
		packet[6] = 'e';
		packet[7] = 'n';
		return 8;
	}
	else if(checksum_problem)
	{
		packet[0] = 'e';
		packet[1] = 'r';
		packet[2] = 'r';
		packet[3] = 'o';
		packet[4] = 'r';
		packet[5] = 'c';
		packet[6] = 'h';
		packet[7] = 'e';
		packet[8] = 'c';
		packet[9] = 'k';
		return 10;
	}
	return 0;
}

Bool client_checksum_check(unsigned char* packet, unsigned char length)
{
	unsigned char checksum = 0;
	int i;
	for(i = 0; i < length - 1; i++)
	{
		checksum = checksum ^ (unsigned char)packet[i];
	}
	checksum = checksum ^ 0xAA;
	checksum = checksum ^ 0xAB;
	checksum = checksum ^ length;
	if(checksum == packet[length - 1])
		return TRUE;
	return FALSE;
}

Bool client_length_test(unsigned char* packet, unsigned char length)
{
	unsigned char temp, PWM_state, servo_state, stimated_length;
	unsigned short PWM_param_num = 0, servo_param_num = 0;
	//get states
	temp = packet[2];
	temp &= 0x3F;
	PWM_state = temp >> 4;
	temp &= 0x0F;
	servo_state = temp;
	//set param_number
	switch(PWM_state)
	{
		case 0:
			PWM_param_num = 0;
			break;
		case 1:
		case 2:
			PWM_param_num = 1;
			break;
		case 3:
			PWM_param_num = 2;
			break;
	}
	switch(servo_state)
	{
		case 0:
			servo_param_num = 0;
			break;
		case 1:
		case 2:
		case 4:
		case 8:
			servo_param_num = 1;
			break;
		case 3:
		case 5:
		case 6:
		case 9:
		case 10:
		case 12:
			servo_param_num = 2;
			break;
		case 7:
		case 11:
		case 13:
		case 14:
			servo_param_num = 3;
			break;
		case 15:
			servo_param_num = 4;
			break;
	}
	stimated_length = 5;
	if(packet[3] != 0)
		stimated_length++;
	stimated_length += PWM_param_num << 1;
	stimated_length += servo_param_num << 1;
	if(length == stimated_length)
		return TRUE;
	return FALSE;
}

Bool client_parse_packet(unsigned char* packet, unsigned char length)
{
	length_problem = FALSE;
	checksum_problem = FALSE;
	if(client_length_test(packet, length) == FALSE)
	{
		length_problem = TRUE;
		return FALSE;
	}
	if(client_checksum_check(packet, length) == FALSE)
	{
		checksum_problem = TRUE;
		return FALSE;
	}
	update_client_param(packet);
	return TRUE;
}

client_write_packet_struct client_get_write_packet_struct()
{
	return client_write_param;
}

client_read_packet_struct client_get_read_packet_struct()
{
	return client_read_param;
}
unsigned char client_get_ADC_channel()
{
	return client_common_param.ADC_Channel;
}

unsigned char client_get_DI()
{
	return client_common_param.Digital_Input;
}

unsigned char client_get_encoder_reset()
{
	return client_common_param.Encoder_Reset;
}

unsigned char client_get_encoder_read()
{
	return client_common_param.Encoder_Read;
}

unsigned char client_get_PWM_state(){
	return client_common_param.PWM_state;
}

unsigned char client_get_servo_state(){
	return client_common_param.Servo_State;
}

unsigned short client_get_PWM_param(uint8_t channel)
{
	return client_write_param.PWM_Param[channel];
}

unsigned short client_get_servo_param(uint8_t channel)
{
	return client_write_param.Servo_Param[channel];
}

unsigned short client_get_digital_output()
{
	return client_common_param.Digital_Output;
}

unsigned short client_get_DO_param(){
	return client_write_param.DO_Param;
}

void client_set_adc_param(uint8_t channel_no, unsigned short value)
{
	client_read_param.ADC_Param[channel_no] = value;
}

void client_set_adc4_param(unsigned short value)
{
	client_read_param.ADC_Param[4] = value;
}

void client_set_adc5_param(unsigned short value)
{
	client_read_param.ADC_Param[5] = value;
}

void client_set_DI_param(unsigned short value, unsigned short channel)
{
	if(value == 0)
		client_read_param.DI_Param &= ~(1 << channel);
	else
		client_read_param.DI_Param |= 1 << channel;
}

void client_set_qenc_read_param(uint8_t channel, short value)
{
	client_read_param.Enc_Read_Param[channel] = value;
}
