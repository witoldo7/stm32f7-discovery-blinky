/* Linker script Upperside for STM32F756NGH6 for ITCM Flash Interface */
/* Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)					  */

OUTPUT_FORMAT ("elf32-littlearm")

/* Memory Spaces Definitions */
MEMORY
{
	/* Instruction TCM SRAM */
	RAM0 (xrw)	: ORIGIN = 0x00000000, LENGTH = 16k		
	/* Data TCM SRAM */
	RAM1 (xrw)	: ORIGIN = 0x20000000, LENGTH = 64k		
	/* Main SRAM1 */
	RAM2 (xrw)	: ORIGIN = 0x20010000, LENGTH = 240k
	/* Main SRAM2 */
	RAM3 (xrw)	: ORIGIN = 0x2004C000, LENGTH = 16k
	/* Backuped SRAM */
	RAM4 (xrw)	: ORIGIN = 0x40024000, LENGTH = 4K
	/* External SDRAM(FMC) */
	RAM5 (xrw)	: ORIGIN = 0xC0000000, LENGTH = 8M
	/* Main Embedded FlashROM */
	ROM (rx)	: ORIGIN = 0x00200000, LENGTH = 1M
	/* External QSPI-ROM */
	QSPI (rx)   : ORIGIN = 0x90000000, LENGTH = 16M
}

/* higher address of the stack bottom */
_estack = ORIGIN(RAM1)+LENGTH(RAM1);

/* higher address of the heap end */
_heap_end = ORIGIN(RAM2)+LENGTH(RAM2)-4;	/* due to 4byte alignments */


/* include the section management sub-script */
INCLUDE "STM32F7xxxx_FLASH.ld"
