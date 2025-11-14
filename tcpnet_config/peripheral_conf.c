#include "peripheral_conf.h"

peripheral_conf peripheral_conf_stored;

unsigned short update_peripheral_conf(unsigned char* packet)
{
	unsigned short i;
	unsigned char temp;
	//parse packet 0..3
	for(i = 0; i < 4; i++)
	{
		peripheral_conf_stored.ethernet_ip[i] = (uint8_t) packet[i];
	}
	//parse packet 4
	temp = packet[4];
	switch(temp >> 4)
	{
		case 0:
			peripheral_conf_stored.usb_config.Baud_rate = 300;
			break;
		case 1:
			peripheral_conf_stored.usb_config.Baud_rate = 600;
			break;
		case 2:
			peripheral_conf_stored.usb_config.Baud_rate = 1200;
			break;
		case 3:
			peripheral_conf_stored.usb_config.Baud_rate = 2400;
			break;
		case 4:
			peripheral_conf_stored.usb_config.Baud_rate = 4800;
			break;
		case 5:
			peripheral_conf_stored.usb_config.Baud_rate = 9600;
			break;
		case 6:
			peripheral_conf_stored.usb_config.Baud_rate = 14400;
			break;
		case 7:
			peripheral_conf_stored.usb_config.Baud_rate = 19200;
			break;
		case 8:
			peripheral_conf_stored.usb_config.Baud_rate = 28800;
			break;
		case 9:
			peripheral_conf_stored.usb_config.Baud_rate = 38400;
			break;
		case 10:
			peripheral_conf_stored.usb_config.Baud_rate = 56000;
			break;
		case 11:
			peripheral_conf_stored.usb_config.Baud_rate = 57600;
			break;
		case 12:
			peripheral_conf_stored.usb_config.Baud_rate = 115200;
			break;
		case 13:
			peripheral_conf_stored.usb_config.Baud_rate = 128000;
			break;
		case 14:
			peripheral_conf_stored.usb_config.Baud_rate = 256000;
			break;
	}
	temp &= 0x0F;
	switch(temp >> 2)
	{
		case 0:
			peripheral_conf_stored.usb_config.Stopbits = UART_STOPBIT_1;
			break;
		case 1:
			peripheral_conf_stored.usb_config.Stopbits = UART_STOPBIT_2;
			break;
	}
	temp &= 0x03;
	switch(temp)
	{
		case 0:
			peripheral_conf_stored.usb_config.Parity = UART_PARITY_NONE;
			break;
		case 1:
			peripheral_conf_stored.usb_config.Parity = UART_PARITY_EVEN;
			break;
		case 2:
			peripheral_conf_stored.usb_config.Parity = UART_PARITY_ODD;
			break;
	}
	
	temp = packet[5];
	switch(temp >> 4)
	{
		case 0:
			peripheral_conf_stored.rs232_config.Baud_rate = 300;
			break;
		case 1:
			peripheral_conf_stored.rs232_config.Baud_rate = 600;
			break;
		case 2:
			peripheral_conf_stored.rs232_config.Baud_rate = 1200;
			break;
		case 3:
			peripheral_conf_stored.rs232_config.Baud_rate = 2400;
			break;
		case 4:
			peripheral_conf_stored.rs232_config.Baud_rate = 4800;
			break;
		case 5:
			peripheral_conf_stored.rs232_config.Baud_rate = 9600;
			break;
		case 6:
			peripheral_conf_stored.rs232_config.Baud_rate = 14400;
			break;
		case 7:
			peripheral_conf_stored.rs232_config.Baud_rate = 19200;
			break;
		case 8:
			peripheral_conf_stored.rs232_config.Baud_rate = 28800;
			break;
		case 9:
			peripheral_conf_stored.rs232_config.Baud_rate = 38400;
			break;
		case 10:
			peripheral_conf_stored.rs232_config.Baud_rate = 56000;
			break;
		case 11:
			peripheral_conf_stored.rs232_config.Baud_rate = 57600;
			break;
		case 12:
			peripheral_conf_stored.rs232_config.Baud_rate = 115200;
			break;
		case 13:
			peripheral_conf_stored.rs232_config.Baud_rate = 128000;
			break;
		case 14:
			peripheral_conf_stored.rs232_config.Baud_rate = 256000;
			break;
	}
	temp &= 0x0F;
	switch(temp >> 2)
	{
		case 0:
			peripheral_conf_stored.rs232_config.Stopbits = UART_STOPBIT_1;
			break;
		case 1:
			peripheral_conf_stored.rs232_config.Stopbits = UART_STOPBIT_2;
			break;
	}
	temp &= 0x03;
	switch(temp)
	{
		case 0:
			peripheral_conf_stored.rs232_config.Parity = UART_PARITY_NONE;
			break;
		case 1:
			peripheral_conf_stored.rs232_config.Parity = UART_PARITY_EVEN;
			break;
		case 2:
			peripheral_conf_stored.rs232_config.Parity = UART_PARITY_ODD;
			break;
	}
	
	temp = packet[6];
	switch(temp >> 4)
	{
		case 0:
			peripheral_conf_stored.rs485_config.Baud_rate = 300;
			break;
		case 1:
			peripheral_conf_stored.rs485_config.Baud_rate = 600;
			break;
		case 2:
			peripheral_conf_stored.rs485_config.Baud_rate = 1200;
			break;
		case 3:
			peripheral_conf_stored.rs485_config.Baud_rate = 2400;
			break;
		case 4:
			peripheral_conf_stored.rs485_config.Baud_rate = 4800;
			break;
		case 5:
			peripheral_conf_stored.rs485_config.Baud_rate = 9600;
			break;
		case 6:
			peripheral_conf_stored.rs485_config.Baud_rate = 14400;
			break;
		case 7:
			peripheral_conf_stored.rs485_config.Baud_rate = 19200;
			break;
		case 8:
			peripheral_conf_stored.rs485_config.Baud_rate = 28800;
			break;
		case 9:
			peripheral_conf_stored.rs485_config.Baud_rate = 38400;
			break;
		case 10:
			peripheral_conf_stored.rs485_config.Baud_rate = 56000;
			break;
		case 11:
			peripheral_conf_stored.rs485_config.Baud_rate = 57600;
			break;
		case 12:
			peripheral_conf_stored.rs485_config.Baud_rate = 115200;
			break;
		case 13:
			peripheral_conf_stored.rs485_config.Baud_rate = 128000;
			break;
		case 14:
			peripheral_conf_stored.rs485_config.Baud_rate = 256000;
			break;
	}
	temp &= 0x0F;
	switch(temp >> 2)
	{
		case 0:
			peripheral_conf_stored.rs485_config.Stopbits = UART_STOPBIT_1;
			break;
		case 1:
			peripheral_conf_stored.rs485_config.Stopbits = UART_STOPBIT_2;
			break;
	}
	temp &= 0x03;
	switch(temp)
	{
		case 0:
			peripheral_conf_stored.rs485_config.Parity = UART_PARITY_NONE;
			break;
		case 1:
			peripheral_conf_stored.rs485_config.Parity = UART_PARITY_EVEN;
			break;
		case 2:
			peripheral_conf_stored.rs485_config.Parity = UART_PARITY_ODD;
			break;
	}
	
	peripheral_conf_stored.rs232_config.Databits = UART_DATABIT_8;
	peripheral_conf_stored.usb_config.Databits = UART_DATABIT_8;
	peripheral_conf_stored.rs485_config.Databits = UART_DATABIT_8;
	//boolean set
	peripheral_conf_stored.adc_flag = TRUE;
	peripheral_conf_stored.spi_flag = FALSE;
	peripheral_conf_stored.pwm_flag = TRUE;
	peripheral_conf_stored.rs232_flag = TRUE;
	peripheral_conf_stored.usb_flag = TRUE;
	peripheral_conf_stored.rs485_flag = TRUE;
	peripheral_conf_stored.dio_flag = TRUE;

	return 7;
}

Bool config_checksum_check(unsigned char* packet)
{
	unsigned char checksum = 0;
	int i;
	for(i = 0; i < CONFIG_PACKET_SIZE - 1; i++)
	{
		checksum = checksum ^ (unsigned char)packet[i];
	}
	if(checksum == packet[CONFIG_PACKET_SIZE - 1])
		return TRUE;
	return FALSE;
}

Bool config_parse_packet(unsigned char* packet)
{
	if(config_checksum_check(packet) == FALSE)
		return FALSE;
	update_peripheral_conf(packet);
	return TRUE;
}

peripheral_conf get_peripheral_conf_struct()
{
	return peripheral_conf_stored;
}

void config_get_ethernet_ip(uint8_t* ethernet_ip)
{
	uint8_t i;
	for(i = 0; i < 4; i++)
		ethernet_ip[i] = peripheral_conf_stored.ethernet_ip[i];
}

uint8_t config_get_adc_working_voltage()
{
	return peripheral_conf_stored.adc_workig_voltage;
}

uint8_t config_get_analog_output_type()
{
	return peripheral_conf_stored.analog_output_type;
}

Bool config_get_adc_flag()
{
	return peripheral_conf_stored.adc_flag;
}

uint8_t config_get_spi_rate()
{
	return peripheral_conf_stored.spi_rate;
}

Bool config_get_spi_flag()
{
	return peripheral_conf_stored.spi_flag;
}

uint16_t config_get_pwm_freq()
{
	return peripheral_conf_stored.pwm_freq;
}

Bool config_get_pwm_flag()
{
	return peripheral_conf_stored.pwm_flag;
}

UART_CFG_Type config_get_rs232_config()
{
	return peripheral_conf_stored.rs232_config;
}

UART_CFG_Type config_get_usb_config()
{
	return peripheral_conf_stored.usb_config;
}

UART_CFG_Type config_get_rs485_config()
{
	return peripheral_conf_stored.rs485_config;
}

Bool config_get_rs232_flag()
{
	return peripheral_conf_stored.rs232_flag;
}

Bool config_get_usb_flag()
{
	return peripheral_conf_stored.usb_flag;
}

Bool config_get_rs485_flag()
{
	return peripheral_conf_stored.rs485_flag;
}
