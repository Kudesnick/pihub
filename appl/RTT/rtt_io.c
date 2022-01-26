/**
 * @file rtt_io.c
 * @author Stulov Tikhon (kudesnick@inbox.ru)
 * @brief Override IO functions of runtime library
 * @version 0.0.1
 * @date 2022-01-26
 * 
 * @copyright Copyright (c) 2022
 * 
 * @details Use this code into innitialisation of system if you want disable input/output buffering.
 * @verbatim
   // Turn off buffers, so I/O occurs immediately
   setvbuf(stdin,  NULL, _IONBF, 0);
   setvbuf(stdout, NULL, _IONBF, 0);
   //setvbuf(stderr, NULL, _IONBF, 0);
 * @endverbatim
 * When _read _write functions called to every input or output byte. Else _read _write functions
 * called after \r or \n symbols only.
 * Example of useed:
 * @verbatim
   if (getc(stdin) == 'H')
   {
     printf("Hello Word!\n");
   }
 * @endverbatim
 */


#include "SEGGER_RTT.h"
#include <stdio.h>

#ifndef LOW_LEVEL_IO
  #define LOW_LEVEL_IO (0)
#endif

const unsigned RTT_ch = 0;

#if (LOW_LEVEL_IO != 0)

int __io_getchar(void)
{
  return SEGGER_RTT_GetKey();
}

int __io_putchar(int ch)
{
  return SEGGER_RTT_PutChar(RTT_ch, ch);
}

#else

int _read(int file, char *ptr, int len)
{
  unsigned result = SEGGER_RTT_Read(RTT_ch, ptr, len);
  return (result != len) ? -1 : len;
}

int _write(int file, char *ptr, int len)
{
  return SEGGER_RTT_Write(RTT_ch, ptr, len);
}

#endif
