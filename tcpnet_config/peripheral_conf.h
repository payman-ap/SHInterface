#include "lpc_types.h"
#include "initparams.h"
#include "lpc17xx_uart.h"

#ifndef PERIPHERAL_CONF_H
#define PERIPHERAL_CONF_H

#define CONFIG_PACKET_SIZE 8
#define SD_FILE_NAME_SIZE 6

//Public Function
Bool config_parse_packet(unsigned char* packet);
peripheral_conf get_peripheral_conf_struct(void);
void config_get_ethernet_ip(uint8_t* ethernet_ip);
uint8_t config_get_adc_working_voltage(void);
uint8_t config_get_analog_output_type(void);
Bool config_get_adc_flag(void);
uint8_t config_get_spi_rate(void);
Bool config_get_spi_flag(void);
uint16_t config_get_pwm_freq(void);
Bool config_get_pwm_flag(void);
UART_CFG_Type config_get_rs232_config(void);
UART_CFG_Type config_get_usb_config(void);
UART_CFG_Type config_get_rs485_config(void);
Bool config_get_rs232_flag(void);
Bool config_get_usb_flag(void);
Bool config_get_rs485_flag(void);

//Private Function
unsigned short update_peripheral_conf(unsigned char* packet);
Bool config_checksum_check(unsigned char* packet);

#endif //PERIPHERAL_CONF_H
