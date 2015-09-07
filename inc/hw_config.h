/********************************************************************************/
/*!
	@file			hw_config.h
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        2.00
    @date           2015.09.01
	@brief          Configure Basis System on STM32F746G-Discovery!

    @section HISTORY
		2015.07.09	V1.00	Start Here.
		2015.09.01	V2.00	Added ITCM Initialization.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H 0x0200

#ifdef __cplusplus
 extern "C" {
#endif

/* General Inclusion */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include "stm32f7xx.h"
#include "stm32f7xx_it.h"
#include "platform_config.h"

/* Function Inclusion */
#include "systick.h"

/* High Level Functions */
#include "uart_support.h"
//#include "ff_func_cfg.h"
//#include "font_if.h"

//#include "display_if.h"
//#include "display_if_support.h"
//#include "ts_fileloads.h"

/* XMSTN */
//#include "xmstn_if.h"
//#include "xmstn_support.h"
/* I2C Devices */
//#include "i2cdev_if.h"
//#include "stts751.h"


/* Macros */
#define countof(a)		(sizeof(a) / sizeof(*(a)))
//#define __ITCM			__attribute__ ((long_call, section (".itcm")))
#define __ITCM			__attribute__ ((section (".itcm")))

/* Externals */
extern __IO uint16_t CmdKey;
extern void Set_System(void);
extern void NVIC_Configuration(void);
extern void initialise_monitor_handles();

#ifdef __cplusplus
}
#endif

#endif  /* __HW_CONFIG_H */
