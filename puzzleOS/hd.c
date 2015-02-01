/*
 *  hd.c
 *  puzzleOS
 *
 *  Created by Dmitry on 31.05.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "hd.h"
#include "ioports.h"

struct HD_PARAM {
	unsigned int cyl;
	unsigned int head;
	unsigned int sect;
} HD0 = {2080, 16, 63};

#define insl(port, buf, nr) \
__asm__ ("cld;rep;insl\n\t"	\
::"d"(port), "D"(buf), "c"(nr))

#define outsl(buf, nr, port) \
__asm__ ("cld;rep;outsl\n\t"	\
::"d"(port), "S" (buf), "c" (nr))


#define ATA_STATUS_BSY	0x80	// device busy, bit 8
#define ATA_STATUS_DRDY	0x40	// device ready, bit 7
#define ATA_STATUS_DF	0x20	// Device Fault, bit 6
#define ATA_STATUS_DSC	0x10	// seek complete
#define ATA_STATUS_DRQ	0x08	// Data Transfer Requested
#define ATA_STATUS_CORR	0x04	// data corrected
#define ATA_STATUS_IDX	0x02	// index mark
#define ATA_STATUS_ERR	0x01	// error


void hd_rw(uint32 lba, uint8 cmd, uint32 count, void *buf) {
	
	//uint32 cyl = lba / (HD0.head * HD0.sect);
//	uint32 head = (lba % (HD0.head * HD0.sect)) / HD0.sect;
//	uint32 sect = (lba % (HD0.head * HD0.sect)) % HD0.sect + 1;	
//
//	while ((inportb(HD_PORT_STATUS) & 0xC0) != 0x40);
//	
//	outportb(HD_PORT_SECT_COUNT, count);
//	outportb(HD_PORT_SECT_NUM, sect);
//	outportb(HD_PORT_CYL_LOW, cyl);
//	outportb(HD_PORT_CYL_HIGH, cyl >> 8);
//	outportb(HD_PORT_DRV_HEAD, 0xA0 | head);
//	outportb(HD_PORT_COMMAND, cmd);
//
//	while (!(inportb(HD_PORT_STATUS) & 0x8));
//	
//	if (cmd == HD_READ)
//		insl(HD_PORT_DATA, buf, count << 7);
//	else if (cmd == HD_WRITE)
//		outsl(buf, count << 7, HD_PORT_DATA);
}

static void hd_setup_DPT() {
	uint8 hd_buff[512] = {0};
	
	hd_buff[0x1be] = 0x80;
	hd_buff[0x1be + 0x04] = FST_FS;
	*(unsigned long *)&hd_buff[0x1be + 0x08] = 1;
	*(unsigned long *)&hd_buff[0x1be + 0x0c] = 85*1024*2; /* 85MB */
	
	hd_buff[0x1ce + 0x04] = FST_SW;
	*(unsigned long *)&hd_buff[0x1ce + 0x08] = 85*1024*2+1;
	*(unsigned long *)&hd_buff[0x1ce + 0x0c] = 16*1024*2; /* 16MB */
	hd_buff[0x1fe] = 0x55;
	hd_buff[0x1ff] = 0xaa;
	hd_rw(0, HD_WRITE, 1, hd_buff);
}

static void hd_erase_DPT() {
	uint8 buff[512] = {0};
	hd_rw(0, HD_WRITE, 1, buff);
}

void hd_verify_DPT(void) {
	uint8 hd_buff[512];
	unsigned i = 0;
	
	kprintf("Checking DPT...");
	
	hd_rw(0, HD_READ, 1, hd_buff);
	
	if ((hd_buff[0x1fe]==0x55) && (hd_buff[0x1ff]==0xaa)) {
		
		kprintf("Ok\n");
		
		unsigned char *p = &hd_buff[0x1be];
		char *s;
		kprintf("   | Bootable | Type      | Start Sector | Capacity \n");
		for (i=0; i<4; ++i) {
			kprintf(" %d ", i);
			/* system indicator at offset 0x04 */
			if (p[0x04] == 0x00) {
				kprintf("| Empty\n");
				p += 16;
			
				continue;
			}
			if (p[0x00] == 0x80)
				s = "| Yes      ";
			else
				s = "| No       ";
			kprintf(s);
			/* system indicator at offset 0x04 */
			if (p[0x04] == FST_FS) {
				kprintf("| Skelix FS ");
			} else if (p[0x04] == FST_SW) {
				kprintf("| Skelix SW ");
			} else
				kprintf("| Unknown   ", *p);
			/* starting sector number */
			//*q++ = *(unsigned long *)&p[0x08];
			kprintf("| 0x%x   ", *(unsigned long*)&p[0x08]);
			/* capacity */
			//*q++ = *(unsigned long*)&p[0x0c];
			kprintf("| %dM\n", (*(unsigned long*)&p[0x0c]*512)>>20);
			p += 16;
		}
	} else {
		kprintf("No DPT found on HD0!\n");
		kprintf("Creating DPT on HD0 automaticly...");
		hd_setup_DPT();
		kprintf("Ok\n");
		hd_verify_DPT();
	}
}


void hd_init() {
	kprintf("Initializing HD\n");
	hd_verify_DPT();
	for(;;);
}

