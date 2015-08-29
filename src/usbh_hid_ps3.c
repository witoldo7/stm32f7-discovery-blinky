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

uint32_t                  ps3_report_data[64];

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

const uint8_t OUTPUT_REPORT_BUFFER[] = {
		0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x02 /*0x00*/, 	//140201 LED1
		0xff, 0x27, 0x10, 0x00, 0x32,
		0xff, 0x27, 0x10, 0x00, 0x32,
		0xff, 0x27, 0x10, 0x00, 0x32,
		0xff, 0x27, 0x10, 0x00, 0x32,
		0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/**
 * @}
 */


/** @defgroup USBH_HID_PS3_Private_Functions
 * @{
 */

static USBH_StatusTypeDef PS3_Command(USBH_HandleTypeDef *phost, uint8_t* data, uint16_t nuint8_ts){
	phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_CLASS;
	phost->Control.setup.b.bRequest = 0x09;

	phost->Control.setup.b.wValue.bw.msb = 0x01;
	phost->Control.setup.b.wValue.bw.lsb = 0x02;
	phost->Control.setup.b.wIndex.w = 0x00;
	phost->Control.setup.b.wLength.w = nuint8_ts;

	return USBH_CtlReq( phost, data , nuint8_ts );

}

static USBH_StatusTypeDef SetupSIXAXISController(USBH_HandleTypeDef *phost)
{
	uint8_t cmd_buf[4];
	cmd_buf[0] = 0x42; // Special PS3 Controller enable commands
	cmd_buf[1] = 0x0c;
	cmd_buf[2] = 0x00;
	cmd_buf[3] = 0x00;

	phost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_CLASS;
	phost->Control.setup.b.bRequest = USB_HID_SET_REPORT;
	phost->Control.setup.b.wValue.w = 0x03f4;
	phost->Control.setup.b.wIndex.w = 0x00;
	phost->Control.setup.b.wLength.w = 4;

	return USBH_CtlReq( phost, cmd_buf , 4 );
}

static USBH_StatusTypeDef InitializeSIXAXISController(USBH_HandleTypeDef *phost)
{
	uint8_t data[17];
	uint8_t RetVal;
	int j;
	for(j = 0;j < 17;j++){
		data[j] = 0;
	}

	phost->Control.setup.b.bmRequestType = 161;
	phost->Control.setup.b.bRequest = 1;
	phost->Control.setup.b.wValue.w = 0x03f2;
	phost->Control.setup.b.wIndex.w = 0x00;
	phost->Control.setup.b.wLength.w = 17;

	return USBH_CtlReq( phost, data , 17 );
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
	USBH_StatusTypeDef s;

	s = InitializeSIXAXISController(phost);
	if( s == USBH_OK ){

		ps3_report_data[0]=0;

		if(HID_Handle->length > sizeof(ps3_report_data))
		{
			HID_Handle->length = sizeof(ps3_report_data);
		}
		HID_Handle->pData = (uint8_t *)ps3_report_data;
		fifo_init(&HID_Handle->fifo, phost->device.Data, HID_QUEUE_SIZE * sizeof(ps3_report_data));

	}

	return s;

}
#if 0
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
		return &mouse_info;
	}
	else
	{
		return NULL;
	}
}
#endif
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
