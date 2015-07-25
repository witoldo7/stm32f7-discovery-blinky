/*
 * dns-netconn.c
 *
 *  Created on: 2015/07/26
 *      Author: spiralray
 */

#include "lwip/opt.h"
#include "main.h"
#include "lcd_log.h"
#include "lwip/dhcp.h"
#include "dns-netconn.h"
#include "ethernetif.h"


void dns_test(){
	LCD_UsrLog ("\nTesting DNS resolver...\n");

	dns_init();

	const char *host = TARGET_HOST;
	struct ip_addr resolved;
	err_t err=netconn_gethostbyname(host, &resolved);

	if( err == ERR_OK ){
		uint8_t iptab[4];
		uint8_t iptxt[20];

		iptab[0] = (uint8_t)(resolved.addr >> 24);
		iptab[1] = (uint8_t)(resolved.addr >> 16);
		iptab[2] = (uint8_t)(resolved.addr >> 8);
		iptab[3] = (uint8_t)(resolved.addr);
		sprintf((char*)iptxt, "%d.%d.%d.%d", iptab[3], iptab[2], iptab[1], iptab[0]);
		LCD_UsrLog ("%s: %s\n", host, iptxt);
	}
	else{
		LCD_UsrLog ("DNS ERROR: %d\n", (int)err );
	}
}
