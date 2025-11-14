/******************************************************************
 *****                                                        *****
 *****  Name: IAP.c                                           *****
 *****  Ver.: 1.0                                             *****
 *****  Date: 28/09/2016                                     *****
 *****  Auth: Payman Azaripasand                              *****
 *****  Func: implements and provides a simple IAP to the     *****
 *****        user                                            *****
 *****                                                        *****
 ******************************************************************/
 
#include "IAP.h"

IAP IAP_entry;

volatile unsigned int flag[1];


unsigned int IAP_PrepareSector(unsigned int start_sector_num, unsigned int end_sector_num)
{
  unsigned int command[5];
  unsigned int result[5];
  command[0] = PREPARE_SECTOR_FOR_WRITE;
  command[1] = start_sector_num;
  command[2] = end_sector_num;
  IAP_entry=(IAP) IAP_LOCATION;
  IAP_entry (command, result);
  return result[0];   
}

unsigned int IAP_EraseSector(unsigned int start_sector_num, unsigned int end_sector_num)
{
  unsigned int command[5];
  unsigned int result[5];
  command[0] = ERASE_SECTOR;
  command[1] = start_sector_num;
  command[2] = end_sector_num;
  command[3] = CCLK;
  IAP_entry=(IAP) IAP_LOCATION;
  IAP_entry (command, result);
  return result[0];   
}


unsigned int IAP_CopyRamToFlash(unsigned int src, unsigned int dst, unsigned int byte_num)
{
  unsigned int command[5];
  unsigned int result[5];
  command[0] = COPY_RAM_TO_FLASH; 
  command[1] = dst;
  command[2] = src;
  command[3] = byte_num;
  command[4] = CCLK;
  IAP_entry=(IAP) IAP_LOCATION;
  IAP_entry (command, result);
  return result[0];   
}

void get_cpu_sn(unsigned int rslt[])
{
  unsigned int cmd = READ_DEVICE_SERIAL_NUMBER;
  IAP_entry=(IAP) IAP_LOCATION;
	IAP_entry (&cmd, rslt);     
}

Bool store_config_flag(unsigned int data)
{
   flag[0] = data;
   if(!IAP_PrepareSector(CONFIG_FLAG_SECTOR, CONFIG_FLAG_SECTOR))
	   if(!IAP_EraseSector(CONFIG_FLAG_SECTOR, CONFIG_FLAG_SECTOR))
          if(!IAP_PrepareSector(CONFIG_FLAG_SECTOR, CONFIG_FLAG_SECTOR))
          {
            if(!IAP_CopyRamToFlash((unsigned int)flag, CONFIG_FLAG_ADDR, 256))
               return TRUE;
          } 
   return FALSE;
}

Bool store_config_packet(unsigned int src, unsigned int byte_num)
{
	if(!IAP_PrepareSector(CONFIG_PACKET_SECTOR, CONFIG_PACKET_SECTOR))
		if(!IAP_EraseSector(CONFIG_PACKET_SECTOR, CONFIG_PACKET_SECTOR))
			if(!IAP_PrepareSector(CONFIG_PACKET_SECTOR, CONFIG_PACKET_SECTOR))
			{
	            if(!IAP_CopyRamToFlash(src, CONFIG_PACKET_ADDR, 256))
	               return TRUE;
	        }
	return FALSE;
}

unsigned int read_config_flag()
{
	return *((unsigned int *)CONFIG_FLAG_ADDR);
}

void load_config_param(unsigned int result[])
{
	uint32_t i;
	for(i = 0; i < DATA_SIZE; i++)
	{
		result[i] = *((unsigned int *)CONFIG_PACKET_ADDR+i);
	}
}



