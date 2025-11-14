/******************************************************************
 *****                                                        *****
 *****  Name: IAP.h                                           *****
 *****  Ver.: 1.0                                             *****
 *****  Date: 28/09/2016                                      *****
 *****  Auth: Payman Azaripasand                              *****
 *****  Func: header-file for IAP.c                           *****
 *****                                                        *****
 ******************************************************************/


#include "lpc_types.h"

#ifndef IAP_H
#define IAP_H

#define CONFIG_FLAG_SECTOR 14
#define CONFIG_FLAG 110
#define CONFIG_FLAG_ADDR   0x0000E000

#define CONFIG_PACKET_SECTOR 15
#define CONFIG_PACKET_ADDR   0x0000F000
#define DATA_SIZE 4

#define PROG_BUFF_SIZE 512
#define PROG_PKT_DATA_SIZE 18  // 16 Bytes

//#define INFO_SECTOR_NO 29
//#define PART_NO_BASE_ADDR 0x00078000
//#define NOBW 256 /* Number of bytes to be written.(Should be 256 | 512 | 1024 | 4096.) */

#define PRODUCT_BYTES_NUM  8
#define VERSION_BYTES_NUM  4
#define SERIAL_BYTES_NUM   4

#define IAP_LOCATION  0x1FFF1FF1
#define CCLK          100000 //CPU clk in KHz

typedef enum
{
  PREPARE_SECTOR_FOR_WRITE  =50,
  COPY_RAM_TO_FLASH			=51,
  ERASE_SECTOR				=52,
  BLANK_CHECK_SECTOR		=53,
  READ_PART_ID				=54,
  READ_BOOT_VER				=55,
  COMPARE					=56,
  REINVOKE_ISP				=57,
  READ_DEVICE_SERIAL_NUMBER =58
}IAP_Command_Code;

typedef void (*IAP)(unsigned int [],unsigned int[]);

Bool store_config_flag(unsigned int);
Bool store_config_packet(unsigned int src, unsigned int byte_num);
unsigned int read_config_flag(void);
void load_config_param(unsigned int result[]);

/* function declaretions */
unsigned int IAP_PrepareSector(unsigned int start_sector_num, unsigned int end_sector_num);
unsigned int IAP_EraseSector(unsigned int start_sector_num, unsigned int end_sector_num);
unsigned int IAP_CopyRamToFlash(unsigned int src, unsigned int dst, unsigned int byte_num);
void get_cpu_sn(unsigned int rslt[]);

#endif //IAP_H
