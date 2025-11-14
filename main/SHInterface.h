/******************************************************************
 *****                                                        *****
 *****  Name: SHInterface.h                                   *****
 *****  Ver.: 1.0                                             *****
 *****  Date: 28/09/2016                                      *****
 *****  Auth: Payman Azaripasand                              *****
 *****  Func: implements a dynamic Software-Hardware          *****
 *****        Interface to be used in Matlab Simulink         *****
 *****                                                        *****
 ******************************************************************/

U8 socket_tcp;
U8 socket_udp;

#define PORT_NUM 1001
#define BLINKLED 0x01  // Command for blink the leds on board

#define BLINKLED 0x01  /* Command for blink the leds on board */
#define SENDLEN  28     /* Number of bytes to send */
#define TCP      0
#define UDP      1

U8 Rem_IP[4] = {192,168,1,31};

uint8_t UART_PINS[UART_CHANNEL_MAX_INDEX]    =  {UART0_PIN,UART1_PIN,UART2_PIN,UART3_PIN};
Bool isConfigFlag = FALSE;

uint8_t QENC_PORTS[CAP_CHANNEL_MAX_INDEX+1] =  {INV_PORT,QENC1_PORT,QENC2_PORT,QENC3_PORT,QENC4_PORT,QENC5_PORT,QENC6_PORT};
uint8_t QENC_PINS[CAP_CHANNEL_MAX_INDEX+1]  =  {INV_PIN, QENC1_PIN, QENC2_PIN, QENC3_PIN, QENC4_PIN, QENC5_PIN, QENC6_PIN };
uint8_t QENC_TIMERS[CAP_CHANNEL_MAX_INDEX+1]=  {INV_TIM, QENC1_TIM, QENC2_TIM, QENC3_TIM, QENC4_TIM, QENC5_TIM, QENC6_TIM };
uint8_t QENC_CAPS[CAP_CHANNEL_MAX_INDEX+1]  =  {INV_CAP, QENC1_CAP, QENC2_CAP, QENC3_CAP, QENC4_CAP, QENC5_CAP, QENC6_CAP };

uint8_t DI_PORTS[DI_MAX_INDEX+1] =  {DI0_PRT, DI1_PRT, DI2_PRT, DI3_PRT, DI4_PRT, DI5_PRT, DI6_PRT, DI7_PRT};
uint8_t DI_PINS[DI_MAX_INDEX+1]  =  {DI0_PIN, DI1_PIN, DI2_PIN, DI3_PIN, DI4_PIN, DI5_PIN, DI6_PIN, DI7_PIN};

uint8_t DO_PORTS[DO_MAX_INDEX+1] =  {DO0_PRT, DO1_PRT, DO2_PRT, DO3_PRT, DO4_PRT, DO5_PRT, DO6_PRT, DO7_PRT};
uint8_t DO_PINS[DO_MAX_INDEX+1]  =  {DO0_PIN, DO1_PIN, DO2_PIN, DO3_PIN, DO4_PIN, DO5_PIN, DO6_PIN, DO7_PIN};


char toggle = 0;
int sumClosed = 0;
void applicate(client_read_packet_struct packet);
