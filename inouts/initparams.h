#ifndef CONFIG_H
#define CONFIG_H

#include "LPC17xx.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_pinsel.h"


#include "lpc_types.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_libcfg_default.h"


typedef struct
{
	uint8_t request_packet[40];
	uint8_t reply_packet[40];
	uint8_t is_request_packet_new;
	uint8_t is_reply_packet_new;
	uint8_t request_size;
	uint8_t reply_size;


} ethernet_packet;

typedef struct
{

	uint8_t 	ethernet_ip[4];

	uint8_t		adc_workig_voltage;

	uint8_t		analog_output_type;

	Bool 		adc_flag;

	uint32_t		spi_rate;
	Bool 		spi_flag;

	uint16_t	pwm_freq;
	Bool 		pwm_flag;

	UART_CFG_Type rs232_config;
	Bool 		rs232_flag;

	UART_CFG_Type usb_config;
	Bool 		usb_flag;

	UART_CFG_Type rs485_config;
	Bool 		rs485_flag;

	Bool 		dio_flag;

} peripheral_conf;


//Timer Config
#define DEBUG_BOARD_TIMER 3

#define LPC_TIM_FUN(x) LPC_TIM ## x
#define LPC_TIM(num) LPC_TIM_FUN(num)

#define TIMER_IRQn_FUN(x) TIMER ## x ##_IRQn
#define TIMER_IRQn(num) TIMER_IRQn_FUN(num)

#define TIM_MR_INT_FUN(x) TIM_MR ## x ##_INT
#define TIM_MR_INT(num) TIM_MR_INT_FUN(num)

#define TIMER_IRQHandler_FUN(x) TIMER ## x ##_IRQHandler(void)
#define TIMER_IRQHandler(num) TIMER_IRQHandler_FUN(num)


//Capture Config
#define CAP_CHANNEL_MAX_INDEX 6
#define CAP_CHANNEL_MIN_INDEX 1

#define QENC1_PORT 0
#define QENC1_PIN  4
#define QENC1_TIM  2
#define QENC1_CAP  0

#define QENC2_PORT 0
#define QENC2_PIN  5
#define QENC2_TIM  2
#define QENC2_CAP  1

#define QENC3_PORT 1
#define QENC3_PIN  18
#define QENC3_TIM  1
#define QENC3_CAP  0

#define QENC4_PORT 1
#define QENC4_PIN  19
#define QENC4_TIM  1
#define QENC4_CAP  1

#define QENC5_PORT 1
#define QENC5_PIN  26
#define QENC5_TIM  0
#define QENC5_CAP  0

#define QENC6_PORT 1
#define QENC6_PIN  27
#define QENC6_TIM  0
#define QENC6_CAP  1

#define INV_PORT 10
#define INV_PIN  100
#define INV_TIM  10
#define INV_CAP  10
//ADC Config

#define ADC0_PIN	23
#define ADC0_PRT	0
#define ADC0_FCN	1

#define ADC1_PIN	24
#define ADC1_PRT	0
#define ADC1_FCN	1

#define ADC2_PIN	25
#define ADC2_PRT	0
#define ADC2_FCN	1

#define ADC3_PIN	26
#define ADC3_PRT	0
#define ADC3_FCN	1

#define ADC4_PIN	30
#define ADC4_PRT	1
#define ADC4_FCN	3

#define ADC5_PIN	31
#define ADC5_PRT	1
#define ADC5_FCN	3

#define ADC_CHANNEL_FUN(x) ADC_CHANNEL_ ## x
#define ADC_CHANNEL(num) ADC_CHANNEL_FUN(num)

#define ADC_ADINTEN_FUN(x) ADC_ADINTEN ## x
#define ADC_ADINTEN(num) ADC_ADINTEN_FUN(num)

//PWM Config

#define MOTOR1		5
#define PWM0_PIN	4
#define PWM0_PRT	2
#define PWM0_FCN	1

#define M1D0_PIN	6
#define M1D1_PIN	7
#define M1D_PRT		2
#define M1D_FCN		0

#define MOTOR2		6
#define PWM1_PIN	5
#define PWM1_PRT	2
#define PWM1_FCN	1

#define M2D0_PIN	8
#define M2D1_PIN	9
#define M2D_PRT		2
#define M2D_FCN		0

#define SERVO1		1
#define SERVO1_PIN	0
#define SERVO1_PRT	2
#define SERVO1_FCN	1

#define SERVO2		2
#define SERVO2_PIN	1
#define SERVO2_PRT	2
#define SERVO2_FCN	1

#define SERVO3		3
#define SERVO3_PIN	2
#define SERVO3_PRT	2
#define SERVO3_FCN	1

#define SERVO4		4
#define SERVO4_PIN	3
#define SERVO4_PRT	2
#define SERVO4_FCN	1

#define MAX_SERVO 20000

//UART Config

#define UART0_PIN 2
#define UART1_PIN 15
#define UART2_PIN 10
#define UART3_PIN 0

#define UART_CHANNEL_MAX_INDEX 4


#define RS485_UART	3
#define USB_UART	2
#define RS232_UART	0

#define RS232_RX_PIN 3
#define RS232_TX_PIN 2
#define RS232_PRT 0
#define RS232_FCN 1

#define USB_RX_PIN 11
#define USB_TX_PIN 10
#define USB_PRT 0
#define USB_FCN 1

#define RS485_RX_PIN 1
#define RS485_TX_PIN 0
#define RS485_PRT 0
#define RS485_FCN 1

#define LPC_UART_FUN(x) LPC_UART ## x
#define LPC_UART(uart_num) LPC_UART_FUN(uart_num)

#define UART_IRQn_FUN(x) UART ## x ##_IRQn
#define UART_IRQn(uart_num) UART_IRQn_FUN(uart_num)

#define UART_IRQHandler_FUN(x) UART ## x ##_IRQHandler(void)
#define UART_IRQHandler(uart_num) UART_IRQHandler_FUN(uart_num)

#define UART_IntReceive_FUN(x) UART ## x ##_IntReceive()
#define UART_IntReceive(uart_num) UART_IntReceive_FUN(uart_num)

#define UART_IntTransmit_FUN(x) UART ## x ##_IntTransmit()
#define UART_IntTransmit(uart_num) UART_IntTransmit_FUN(uart_num)

#define UART_IntErr_FUN(x) UART ## x ##_IntErr
#define UART_IntErr(uart_num) UART_IntErr_FUN(uart_num)

/* buffer size definition */
#define UART_RING_BUFSIZE 256

typedef struct
{
    __IO uint32_t tx_head;                /*!< UART Tx ring buffer head index */
    __IO uint32_t tx_tail;                /*!< UART Tx ring buffer tail index */
    __IO uint32_t rx_head;                /*!< UART Rx ring buffer head index */
    __IO uint32_t rx_tail;                /*!< UART Rx ring buffer tail index */
    __IO uint8_t  tx[UART_RING_BUFSIZE];  /*!< UART Tx data ring buffer */
    __IO uint8_t  rx[UART_RING_BUFSIZE];  /*!< UART Rx data ring buffer */
} UART_RING_BUFFER_T;


/* Buf mask */
#define __BUF_MASK (UART_RING_BUFSIZE-1)
/* Check buf is full or not */
#define __BUF_IS_FULL(head, tail) ((tail&__BUF_MASK)==((head+1)&__BUF_MASK))
/* Check buf will be full in next receiving or not */
#define __BUF_WILL_FULL(head, tail) ((tail&__BUF_MASK)==((head+2)&__BUF_MASK))
/* Check buf is empty */
#define __BUF_IS_EMPTY(head, tail) ((head&__BUF_MASK)==(tail&__BUF_MASK))
/* Reset buf */
#define __BUF_RESET(bufidx) (bufidx=0)
#define __BUF_INCR(bufidx)  (bufidx=(bufidx+1)&__BUF_MASK)

//PWM Config

#define SPI_SCK_PIN 15
#define SPI_IO_PIN 16
#define SPI_MISO_PIN 17
#define SPI_MSOI_PIN 18

#define SPI_PRT 0
#define SPI_FCN 3
#define SPI_IO_FCN 0

//DIO Config
#define DI_MAX_INDEX 7
#define DO_MAX_INDEX 7

#define DI0_PIN	6
#define DI0_PRT	0
#define DI0_FCN	0

#define DI1_PIN	7
#define DI1_PRT	0
#define DI1_FCN	0

#define DI2_PIN	8
#define DI2_PRT	0
#define DI2_FCN	0

#define DI3_PIN	9
#define DI3_PRT	0
#define DI3_FCN	0

#define DO0_PIN	26
#define DO0_PRT	3
#define DO0_FCN	0

#define DO1_PIN	25
#define DO1_PRT	3
#define DO1_FCN	0

#define DO2_PIN	28
#define DO2_PRT	1
#define DO2_FCN	0

#define DO3_PIN	29
#define DO3_PRT	1
#define DO3_FCN	0

//LEDs Config
#define DO4_PIN	25
#define DO4_PRT	1
#define DO4_FCN	0

#define DO5_PIN	22
#define DO5_PRT	1
#define DO5_FCN	0

#define DO6_PIN	30
#define DO6_PRT	0
#define DO6_FCN	0

#define DO7_PIN	29
#define DO7_PRT	0
#define DO7_FCN	0


//Switches Config
#define DI4_PIN	10
#define DI4_PRT	2
#define DI4_FCN	0

#define DI5_PIN	11
#define DI5_PRT	2
#define DI5_FCN	0

#define DI6_PIN	12
#define DI6_PRT	2
#define DI6_FCN	0

#define DI7_PIN	13
#define DI7_PRT	2
#define DI7_FCN	0

//Config LED
#define CONFIG_LED_PIN 6
#define CONFIG_LED_PRT 0


#endif
