/********************************************************************************/
/*!
	@file			systick.h
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        1.00
    @date           2015.07.09
	@brief          Based on ST Microelectronics's Sample Thanks!
					For STM32F7 Only!

    @section HISTORY
		2015.07.09	V1.00	Start Here.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/
#ifndef __SYSTICK_H
#define __SYSTICK_H 0x0100

#ifdef __cplusplus
 extern "C" {
#endif

/* General Inclusion */
#include "platform_config.h"

/* Systick Interval MilliSecond order (in Hz) */
#define INTERVAL		1000UL
/* Timer Interval MicroSecond order (in Hz) */
#define USEC_INTERVAL 	1000000UL

/* Inline Static Function */
extern __IO uint32_t TimingDelay;
static inline void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

/* Externals */
extern void _delay_ms(__IO uint32_t mSec);
extern void _delay_us(__IO uint32_t uSec);
extern void Delay(__IO uint32_t uCount);
extern void SysTickInit(__IO uint32_t interval);

#ifdef __cplusplus
}
#endif

#endif	/* __SYSTICK_H */
