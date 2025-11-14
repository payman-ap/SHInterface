
#include "lpc_types.h"

#ifndef DATA_PROVIDER_H
#define DATA_PROVIDER_H

#define CLIENT_PACKET_SIZE 4

typedef struct
{
	unsigned char ADC_Channel;
	unsigned char Digital_Input;
	unsigned char Encoder_Reset;
	unsigned char Encoder_Read;
	unsigned char Digital_Output;
	unsigned char PWM_state;
	unsigned char Servo_State;
} client_common_packet_struct;


typedef struct
{
	unsigned short ADC_Param[6];
	unsigned char DI_Param;
	short Enc_Read_Param[3];
} client_read_packet_struct;


typedef struct
{
	unsigned short PWM_Param[2];
	unsigned short Servo_Param[4];
	unsigned char DO_Param;
} client_write_packet_struct;

unsigned short update_client_param(unsigned char* packet);
unsigned short generate_reply_packet(unsigned char* packet);
unsigned short generate_bad_request_packet(unsigned char* packet);
Bool client_checksum_check(unsigned char* packet, unsigned char length);
Bool client_parse_packet(unsigned char* packet, unsigned char length);
Bool client_length_test(unsigned char* packet, unsigned char length);
client_write_packet_struct client_get_write_packet_struct(void);
client_read_packet_struct client_get_read_packet_struct(void);
unsigned char client_get_ADC_channel(void);
unsigned char client_get_DI(void);
unsigned char client_get_encoder_reset(void);
unsigned char client_get_encoder_read(void);
unsigned char client_get_PWM_state(void);
unsigned char client_get_servo_state(void);
unsigned short client_get_PWM_param(uint8_t channel);
unsigned short client_get_servo_param(uint8_t channel);
unsigned short client_get_digital_output(void);
unsigned short client_get_DO_param(void);
void client_set_adc_param(uint8_t channel_no, unsigned short value);
void client_set_adc1_param(unsigned short value);
void client_set_adc2_param(unsigned short value);
void client_set_adc3_param(unsigned short value);
void client_set_adc4_param(unsigned short value);
void client_set_adc5_param(unsigned short value);
void client_set_DI_param(unsigned short value, unsigned short channel);
void client_set_qenc_read_param(uint8_t channel, short value);

#endif //DATA_PROVIDER_H
