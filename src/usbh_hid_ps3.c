/**
  ******************************************************************************
  * @file    usbh_hid_mouse.c
  * @author  MCD Application Team
  * @version V3.2.1
  * @date    26-June-2015
  * @brief   This file is the application layer for USB Host HID PS3 Handling.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbh_hid_ps3.h"
#include "usbh_hid_parser.h"


/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_HID_CLASS
  * @{
  */

/** @defgroup USBH_HID_MOUSE
  * @brief    This file includes HID Layer Handlers for USB Host HID class.
  * @{
  */

/** @defgroup USBH_HID_MOUSE_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_HID_MOUSE_Private_Defines
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_HID_MOUSE_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_HID_MOUSE_Private_FunctionPrototypes
  * @{
  */
static USBH_StatusTypeDef USBH_HID_PS3Decode(USBH_HandleTypeDef *phost);

/**
  * @}
  */


/** @defgroup USBH_HID_MOUSE_Private_Variables
  * @{
  */
HID_PS3_Info_TypeDef    ps3_info;
uint8_t                  ps3_report_data[64];

/* Structures defining how to access items in a HID mouse report */
/* Access button 1 state. */
static const HID_Report_ItemTypedef prop_b1={
  (uint8_t *)ps3_report_data+0, /*data*/
  1,     /*size*/
  0,     /*shift*/
  0,     /*count (only for array items)*/
  0,     /*signed?*/
  0,     /*min value read can return*/
  1,     /*max value read can return*/
  0,     /*min value device can report*/
  1,     /*max value device can report*/
  1      /*resolution*/
};

/* Access button 2 state. */
static const HID_Report_ItemTypedef prop_b2={
  (uint8_t *)ps3_report_data+0, /*data*/
  1,     /*size*/
  1,     /*shift*/
  0,     /*count (only for array items)*/
  0,     /*signed?*/
  0,     /*min value read can return*/
  1,     /*max value read can return*/
  0,     /*min value device can report*/
  1,     /*max value device can report*/
  1      /*resolution*/
};

/* Access button 3 state. */
static const HID_Report_ItemTypedef prop_b3={
  (uint8_t *)ps3_report_data+2, /*data*/
  1,     /*size*/
  0,     /*shift*/
  0,     /*count (only for array items)*/
  0,     /*signed?*/
  0,     /*min value read can return*/
  1,     /*max value read can return*/
  0,     /*min vale device can report*/
  1,     /*max value device can report*/
  1      /*resolution*/
};

/* Access x coordinate change. */
static const HID_Report_ItemTypedef prop_x={
  (uint8_t *)ps3_report_data+3, /*data*/
  8,     /*size*/
  0,     /*shift*/
  0,     /*count (only for array items)*/
  1,     /*signed?*/
  0,     /*min value read can return*/
  0xFFFF,/*max value read can return*/
  0,     /*min vale device can report*/
  0xFFFF,/*max value device can report*/
  1      /*resolution*/
};

/* Access y coordinate change. */
static const HID_Report_ItemTypedef prop_y={
  (uint8_t *)ps3_report_data+4, /*data*/
  8,     /*size*/
  0,     /*shift*/
  0,     /*count (only for array items)*/
  1,     /*signed?*/
  0,     /*min value read can return*/
  0xFFFF,/*max value read can return*/
  0,     /*min vale device can report*/
  0xFFFF,/*max value device can report*/
  1      /*resolution*/
};


/**
  * @}
  */


/** @defgroup USBH_HID_MOUSE_Private_Functions
  * @{
  */

USBH_StatusTypeDef enable_sixaxis(USBH_HandleTypeDef *phost) { //Command used to enable the Dualshock 3 and Navigation controller to send data via USB
	uint8_t cmd_buf[4];
	cmd_buf[0] = 0x42; // Special PS3 Controller enable commands
	cmd_buf[1] = 0x0c;
	cmd_buf[2] = 0x00;
	cmd_buf[3] = 0x00;

	phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_CLASS;
	phost->Control.setup.b.bRequest = USB_HID_SET_REPORT;
	phost->Control.setup.b.wValue.bw.msb = 0xF4;
	phost->Control.setup.b.wValue.bw.lsb = 0x03;
	phost->Control.setup.b.wIndex.w = 0x00;
	phost->Control.setup.b.wLength.w = 4;

	return USBH_CtlReq( phost, cmd_buf , 4 );
}

/**
  * @brief  USBH_HID_PS3Init
  *         The function init the HID mouse.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_HID_PS3Init(USBH_HandleTypeDef *phost)
{
  HID_HandleTypeDef *HID_Handle =  (HID_HandleTypeDef *) phost->pActiveClass->pData;

  ps3_info.x=0;
  ps3_info.y=0;
  ps3_info.buttons[0]=0;
  ps3_info.buttons[1]=0;
  ps3_info.buttons[2]=0;

  ps3_report_data[0]=0;

  if(HID_Handle->length > sizeof(ps3_report_data))
  {
    HID_Handle->length = sizeof(ps3_report_data);
  }
  HID_Handle->pData = (uint8_t *)ps3_report_data;
  fifo_init(&HID_Handle->fifo, phost->device.Data, HID_QUEUE_SIZE * sizeof(ps3_report_data));

  return USBH_OK;
}

/**
  * @brief  USBH_HID_GetPS3Info
  *         The function return mouse information.
  * @param  phost: Host handle
  * @retval mouse information
  */
HID_PS3_Info_TypeDef *USBH_HID_GetPS3Info(USBH_HandleTypeDef *phost)
{
 if(USBH_HID_PS3Decode(phost)== USBH_OK)
 {
  return &ps3_info;
 }
 else
 {
  return NULL;
 }
}

/**
  * @brief  USBH_HID_PS3Decode
  *         The function decode mouse data.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_HID_PS3Decode(USBH_HandleTypeDef *phost)
{
  HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef *) phost->pActiveClass->pData;

  if(HID_Handle->length == 0)
  {
    return USBH_FAIL;
  }
  /*Fill report */
  if(fifo_read(&HID_Handle->fifo, &ps3_report_data, HID_Handle->length) ==  HID_Handle->length)
  {

    /*Decode report */
    ps3_info.x = (int16_t )HID_ReadItem((HID_Report_ItemTypedef *) &prop_x, 0);
    ps3_info.y = (int16_t )HID_ReadItem((HID_Report_ItemTypedef *) &prop_y, 0);

    ps3_info.buttons[0]=(uint8_t)HID_ReadItem((HID_Report_ItemTypedef *) &prop_b1, 0);
    ps3_info.buttons[1]=(uint8_t)HID_ReadItem((HID_Report_ItemTypedef *) &prop_b2, 0);
    ps3_info.buttons[2]=(uint8_t)HID_ReadItem((HID_Report_ItemTypedef *) &prop_b3, 0);

    return USBH_OK;
  }
  return   USBH_FAIL;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
