/*
 *  atapi.h
 *  puzzleOS
 *
 *  Created by Dmitry on 19.05.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#ifndef _ATAPI_H_
#define _ATAPI_H_

#define ATA_PRIMARY_IDE		0x1F0
#define ATA_SECONDERY_IDE	0x170

#define ATA_PORT_DATA		0x0
#define ATA_PORT_ERROR		0x1
#define ATA_PORT_SECT_COUNT	0x2
#define ATA_PORT_SECT_NUM	0x3
#define ATA_PORT_CYL_LOW	0x4
#define ATA_PORT_CYL_HIGH	0x5
#define	ATA_PORT_DRV_HEAD	0x6
#define ATA_PORT_STATUS		0x7
#define ATA_PORT_COMMAND	0x7

typedef struct atapi_device {
	uint32 initialized;
	char name[40];
	uint32 packet_size;
	uint16 cmd_reg;
	uint32 master;
} atapi_device_t;

void atapi_init();

#endif
