/********************************************************************************/
/*!
	@file			platform_config.h
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        1.00
    @date           2015.07.09
	@brief          Configure Basis System on STM32F746G-Discovery!.

    @section HISTORY
		2015.07.09	V1.00	Start Here.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H 0x0100

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_conf.h"

#ifndef BRR
typedef struct
{
  __IO uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
  __IO uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
  __IO uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
  __IO uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  __IO uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
  __IO uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
  __IO uint16_t BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
  __IO uint16_t BRR;      /*!< GPIO port bit set/reset register,      Address offset: 0x1A      */
  __IO uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
  __IO uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} GPIO_OldTypeDef;
#endif

/* Exported types ------------------------------------------------------------*/
#if defined(USE_STM32746G_DISCOVERY)
 /* STM32F746G-Discovery */
 #include "stm32746g_discovery.h"
 #include "stm32746g_discovery_sdram.h"
 #include "stm32746g_discovery_qspi.h"

 /* FatFs includes component */
 #include "ff_gen_drv.h"
 #include "sd_diskio.h"

 /* LED Definitions(ArduinoUNO Style SCK/LED Pin) */
 #define GPIO_LED               		(GPIOI)
 #define GPIO_LED_L               		((GPIO_OldTypeDef *)GPIOI)
 #define RCC_AHBPeriph_GPIO_LED			(RCC_AHB1ENR_GPIOIEN)
 #define LED_D1      					(GPIO_PIN_1)
 #define LED_IO_HI(x)					(GPIO_LED_L->BSRR = x)
 #define LED_IO_LO(x)					(GPIO_LED_L->BRR = x)
 #define LED_OFF(x)						LED_IO_LO(x)		/* Active High */
 #define LED_ON(x)						LED_IO_HI(x)		/* Active High */
 #define LED_D1_OFF()					LED_OFF(LED_D1)
 #define LED_D1_ON()					LED_ON(LED_D1)
#else
 #error "Select Your EVAL Board!"
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif	/* __PLATFORM_CONFIG_H */
