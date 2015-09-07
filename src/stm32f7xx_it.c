/********************************************************************************/
/*!
	@file			stm32f7xx_it.c
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        1.00
    @date           2015.07.09
	@brief          Cortex-M7 Processor Exceptions Handlers.				@n
					And STM32F7xx Peripherals Interrupt Handlers.			@n
					Device Dependent Section.

    @section HISTORY
		2015.07.09	V1.00	Start Here.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_it.h"
#include "hw_config.h"

#ifdef USE_EMWIN
#include "GUI.h"
#endif

#ifdef OS_SUPPORT
 #include "cmsis_os.h"
#endif

/* Defines -------------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/
#if defined(USE_EMWIN) & !defined(OS_SUPPORT)
extern volatile GUI_TIMER_TIME OS_TimeMS;
#endif

/* Constants -----------------------------------------------------------------*/

/* Function prototypes -------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M7 Processor Exceptions Handlers                         */
/******************************************************************************/

/**************************************************************************/
/*!
    @brief	Handles NMI exception.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
void NMI_Handler(void)
{
}


/**************************************************************************/
/*!
    @brief	Handles Hard Fault exception.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}


/**************************************************************************/
/*!
    @brief	Handles Memory Manage exception.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}


/**************************************************************************/
/*!
    @brief	Handles Bus Fault exception.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}


/**************************************************************************/
/*!
    @brief	Handles Usage Fault exception.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}


/**************************************************************************/
/*!
    @brief	Handles SVCall exception.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
#ifndef OS_SUPPORT
void SVC_Handler(void)
{
}
#endif

/**************************************************************************/
/*!
    @brief	Handles Debug Monitor exception.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
void DebugMon_Handler(void)
{
}


/**************************************************************************/
/*!
    @brief	Handles PendSVC exception.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
#ifndef OS_SUPPORT
void PendSV_Handler(void)
{
}
#endif

/**************************************************************************/
/*!
    @brief	Handles SysTick Handler.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
void SysTick_Handler(void)
{
#ifdef OS_SUPPORT
  osSystickHandler();
#endif

#if defined(USE_EMWIN) & !defined(OS_SUPPORT)
  OS_TimeMS++;
#endif

	/* every 1mSec */
	/* SysTick */
	TimingDelay_Decrement();

#if defined(USE_HAL_DRIVER)
	/* STM32 HAL Driver */
	HAL_IncTick();
#endif

#if defined(USE_HWKEY_INPUT_SUPPORT)
	/* key inputs */
	JoyInp_Chk();
#endif

}

/******************************************************************************/
/*                 STM32F7xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32F7xx_xx.s).                                            */
/******************************************************************************/


/* End Of File ---------------------------------------------------------------*/
