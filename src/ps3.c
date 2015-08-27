/*
 * ps3.c
 *
 *  Created on: 2015/08/27
 *      Author: spiralray
 */

#include "main.h"

extern uint8_t ps3_report_data[64];

void HID_PS3MenuProcess(){

}

void USR_PS3_ProcessData(HID_PS3_Info_TypeDef *data)
{
	printf("0x%02x 0x%02x\n",ps3_report_data[2], ps3_report_data[3]);
}
