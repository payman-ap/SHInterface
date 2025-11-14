/******************************************************************
 *****                                                        *****
 *****  Name: SHInterface.c                                   *****
 *****  Ver.: 1.0                                             *****
 *****  Date: 28/09/2016                                      *****
 *****  Auth: Payman Azaripasand                              *****
 *****  Func: implements a dynamic Software-Hardware          *****
 *****        Interface to be used in Matlab Simulink         *****
 *****                                                        *****
 ******************************************************************/

#include <RTL.h>
#include <stdio.h>
#include <LPC17xx.h>
#include <string.h>
#include "data_provider.h"
#include "encoder_read.h"
#include "pwm_write.h"
#include "initparams.h"
#include "peripheral_conf.h"
#include "exti.h"
#include "IAP.h"
#include "analog_to_digit_conv.h"
#include "digit_out.h"
#include "peripherals.h"
#include "lpc17xx_timer.h"
#include "SHInterface.h"





/*--------------------------- initialization ------------------------------------------*/

static void init () {
  
  SystemInit();

  LPC_GPIO1->FIODIR   |= 0xB0000000;
  LPC_GPIO2->FIODIR   |= 0x0000007C;

  LPC_PINCON->PINSEL4 &= 0xFFFFFFF0;
  LPC_PINCON->PINSEL4 |= 0x0000000A;
  LPC_UART1->LCR = 0x83;
  LPC_UART1->DLL = 9;                         
  LPC_UART1->FDR = 0x21;    
  LPC_UART1->DLM = 0;
  LPC_UART1->LCR = 0x03;

  SysTick->LOAD = (SystemCoreClock / 10) - 1;
  SysTick->CTRL = 0x05;
}


/*--------------------------- TCP/IP send --------------------------------------*/

void send_data (U8 p2val) {
  U8 *sendbuf;
  U8 p2;
	U16 len;
	
  /* TCP */
  if (socket_tcp != 0) {
    /* Start Connection */
    p2 = p2val;
    switch (tcp_get_state(socket_tcp)) {  // checking whether the tcp socket is free
      case TCP_STATE_FREE:
      case TCP_STATE_CLOSED:
        tcp_connect (socket_tcp, Rem_IP, PORT_NUM, 0);    // connect to the tcp socket
				sumClosed++;
        break;
      case TCP_STATE_CONNECT:   // waiting for tcp connection confirmation
        if (tcp_check_send (socket_tcp) == __TRUE) {
          sendbuf = tcp_get_buf(SENDLEN);   // dedicate buffer for send data
					if(p2 != 0)
					{
						len = generate_reply_packet(sendbuf);
					}
					else
					{
						len = generate_bad_request_packet(sendbuf);
					}
          tcp_send (socket_tcp, sendbuf, len);  //  send the data in the send buffer
        }
        break;
    }
  }
}



/*--------------------------- timer ------------------------------------*/

static void timer_poll () {

  if (SysTick->CTRL & 0x10000) {
    timer_tick ();
  }
}


/*--------------------------- fputc -----------------------------------------*/

int fputc(int ch, FILE *f)  {

  if (ch == '\n')  {
    while (!(LPC_UART1->LSR & 0x20));
    LPC_UART1->THR = 0x0D;
  }
  while (!(LPC_UART1->LSR & 0x20));
  LPC_UART1->THR = (ch & 0xFF);
  return (ch);
}


/*--------------------------- LEDs ---------------------------------------*/

void LED_out (U32 val) {
  const U8 led_pos[8] = { 28, 29, 31, 2, 3, 4, 5, 6 };
  U32 i,mask;

  for (i = 0; i < 8; i++) {
    mask = 1 << led_pos[i];
    if (val & (1<<i)) {
      if (i < 3) LPC_GPIO1->FIOSET = mask;
      else       LPC_GPIO2->FIOSET = mask;
    }
    else {
      if (i < 3) LPC_GPIO1->FIOCLR = mask;
      else       LPC_GPIO2->FIOCLR = mask;
    }
  }
}


/*--------------------------- Process received data  ------------------------*/

void procrec (U8 *buf) {
	if(client_parse_packet(buf + 3, buf[2]))
	{
			applicate(client_get_read_packet_struct());
			send_data((U8)8);
	}
	else
	{
			send_data((U8)0);
	}
}

/*--------------------------- UDP socket ------------------------------------*/

U16 udp_callback (U8 soc, U8 *rip, U16 rport, U8 *buf, U16 len) {
  rip  = rip;
  rport= rport;
  len  = len;

  if (soc != socket_udp) {
    return (0);
  }
  procrec(buf);
  return (0);
}




/*--------------------------- TCP socket ------------------------------------*/

U16 tcp_callback (U8 soc, U8 evt, U8 *ptr, U16 par) {  
	
  par = par;
  if (soc != socket_tcp) {
    return (0);
  }

  switch (evt) {
    case TCP_EVT_DATA:
      
      procrec(ptr);
	  break;

    case TCP_EVT_CONREQ:
      
      return (1);

    case TCP_EVT_CONNECT:
      
      return (1);
  }
  return (0);
}


/*--------------------------- LED Function ------------------------------------*/

void init_config_led()
{
	GPIO_SetDir(CONFIG_LED_PRT, 1<<CONFIG_LED_PIN, 1);
	GPIO_SetDir(0, 1<<22, 1); 
	GPIO_SetValue(CONFIG_LED_PRT, 1<<CONFIG_LED_PIN);
	GPIO_ClearValue(0, 1<<22); 
}

void config_led_blink()
{
	uint32_t i;
	GPIO_ClearValue(CONFIG_LED_PRT, 1<<CONFIG_LED_PIN);
	for(i = 0; i < 1000000; i++);
	GPIO_SetValue(CONFIG_LED_PRT, 1<<CONFIG_LED_PIN);
	for(i = 0; i < 1000000; i++);
}


Bool isLoadedPacket(unsigned char* string)
{
	if(string[0] != 'B')
		return FALSE;
	if(string[1] != 9)
		return FALSE;
	if(string[2] != 'L')
		return FALSE;
	if(string[3] != 'o')
		return FALSE;
	if(string[4] != 'a')
		return FALSE;
	if(string[5] != 'd')
		return FALSE;
	if(string[6] != 'D')
		return FALSE;
	if(string[7] != 'a')
		return FALSE;
	if(string[8] != 't')
		return FALSE;
	if(string[9] != 'a')
		return FALSE;
	if(string[10] != ']')
		return FALSE;
	return TRUE;
}


void applicate(client_read_packet_struct packet)
{
	uint8_t i=0;
	uint32_t j = 0;

	for(i=0; i<2; i++){
		if((client_get_PWM_state() & ((uint32_t)(1)) << i))
			set_motor_pw(i,client_get_PWM_param(i));
	}

	for(i=0; i<4; i++){
		if((client_get_servo_state() & ((uint32_t)(1)) << i))
		{
			if(i != 2 && i != 3)
				set_servo_pw(i+1,MAX_SERVO - client_get_servo_param(i));
			if(i == 2)
				set_servo_pw(4,MAX_SERVO - client_get_servo_param(i));
			if(i == 3)
				set_servo_pw(3,MAX_SERVO - client_get_servo_param(i));
		}
	}


	for(i=0; i<6; i++)
	{
		if((client_get_ADC_channel() & ((uint32_t)(1)) << i))
		{
			client_set_adc_param(i,get_adc_value_channel(i));
			for(j = 0; j <= 100; j++);
		}
	}

	for(i=0; i<8; i++)
	{
		if((client_get_digital_output() & ((uint32_t)(1)) << i))
			write_DO(i,(client_get_DO_param() & ((uint32_t)(1)) << i));

		if((client_get_DI() & ((uint32_t)(1)) << i))
			client_set_DI_param(read_DI(i), i);
	}


	for(i=0; i<=3; i++)
	{
		if((client_get_encoder_reset() & ((uint32_t)(1)) << i))
			reset_qencoder(i);

		if((client_get_encoder_read() & ((uint32_t)(1)) << i))
			client_set_qenc_read_param(i,get_qenc_value(i));
	}
}




/*--------------------------- main ------------------------------------------*/



int main (void) {
	int i=0;
	uint8_t string[30];
	init();
	SystemInit();
	init_config_led();
	init_ext_interrupts();
	string[0] = 'S';
	string[1] = 192;
	string[2] = 168;
	string[3] = 1;
	string[4] = 154;
	string[5] = 0;
	string[6] = 0;
	string[7] = 0;
	string[8] = 108;
	string[9] = 255;
	store_config_packet((unsigned int)string, 10);
  while(1){
			load_config_param((unsigned int *)string);
			update_peripheral_conf(string + 1);
			conf_peris(get_peripheral_conf_struct());
			if(Rem_IP[0] == get_peripheral_conf_struct().ethernet_ip[0])
				if(Rem_IP[1] == get_peripheral_conf_struct().ethernet_ip[1])
					if(Rem_IP[2] == get_peripheral_conf_struct().ethernet_ip[2])
						Rem_IP[3] = get_peripheral_conf_struct().ethernet_ip[3];
			init_TcpNet();
			
			socket_udp = udp_get_socket(0, UDP_OPT_SEND_CS | UDP_OPT_CHK_CS, udp_callback);
			if(socket_udp != 0)
			{
				udp_open(socket_udp, PORT_NUM);
			}
			
			socket_tcp = tcp_get_socket (TCP_TYPE_CLIENT_SERVER, 0, 10, tcp_callback);
			if(socket_tcp != 0)
			{
				tcp_listen(socket_tcp, PORT_NUM);
			}
			while(!isConfigFlag)
			{
				timer_poll ();
				main_TcpNet ();
			}
		
	}
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/