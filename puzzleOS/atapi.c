/*
 *  atapi.c
 *  puzzleOS
 *
 *  Created by Dmitry on 19.05.09.
 *  Copyright 2009 Dmitry Obukhov. All rights reserved.
 *
 */

#include "puzzle.h"
#include "ioports.h"

#include "atapi.h"

#define BS8(x) (x)
#define BS16(x) (((x&0xFF00)>>8)|((x&0x00FF)<<8))
#define BS32(x) (((x&0xFF000000)>>24)|((x&0x00FF0000)>>8)|((x&0x0000FF00)<<8)|((x&0x000000FF)<<24))

#define BIG_TO_HOST8(x) BS8((x))
#define BIG_TO_HOST16(x) BS16((x))
#define BIG_TO_HOST32(x) BS32((x))
#define BIG_TO_HOST64(x) BS64((x))

#define HOST_TO_BIG8(x) BS8((x))
#define HOST_TO_BIG16(x) BS16((x))
#define HOST_TO_BIG32(x) BS32((x))
#define HOST_TO_BIG64(x) BS64((x))


#define ATA_STATUS_PORT 0x1F7

#define ATA_STATUS_BSY	0x80	// device busy, bit 8
#define ATA_STATUS_DRDY	0x40	// device ready, bit 7
#define ATA_STATUS_DF	0x20	// Device Fault, bit 6
#define ATA_STATUS_DSC	0x10	// seek complete
#define ATA_STATUS_DRQ	0x08	// Data Transfer Requested
#define ATA_STATUS_CORR	0x04	// data corrected
#define ATA_STATUS_IDX	0x02	// index mark
#define ATA_STATUS_ERR	0x01	// error

#define ATA_MAX_REQUESTS 100

atapi_device_t atapi_device;

uint16 buff[256];

static int atapi_send_command(uint32 nRespBytes, void *respBuff, uint32 nPackBytes, void *packet, int bWrite);
static int atapi_getCapacityInternal(size_t *blockNumber, size_t *blockSize);

int ata_identify_dev(int ide_num, int master, char *dev_name) {
	int i = 0;
	
	int ide_ctrl = (ide_num == 1) ? ATA_PRIMARY_IDE : ATA_SECONDERY_IDE;
	
	// spin-up
    outportb(ide_ctrl + 6, 0x00);
	
	
    outportb(ide_ctrl + 6, (master)?0xA0:0xB0);
    for (i = 0; i < 5; i++)
        inportb(ide_ctrl + 7);
	
    // Send IDENTIFY
    uint8 status = inportb(ide_ctrl + 7);
    outportb(ide_ctrl + 7, 0xEC);
	
	
    status = inportb(ide_ctrl + 7);
    if (status == 0)
        return 0;
	
    // Poll until BSY is clear and either ERR or DRQ are set
    while ( ((status&0x80) != 0) && ((status&0x9) == 0) )
        status = inportb(ide_ctrl + 7);
	
    // If ERR ststus
    if (status & 0x1)
        return 0;
	
    for (i = 0; i < 256; i++)
        buff[i] = inportw(ide_ctrl + 0);
    
    status = inportb(ide_ctrl + 7);
	
    // Get the device name
    for (i = 0; i < 20; i++) {
        dev_name[i*2] = buff[0x1B+i] >> 8;
        dev_name[i*2+1] = buff[0x1B+i] & 0xFF;
    }
    for (i = 39; i > 0; i--) {
        if (dev_name[i] != ' ')
            break;
        dev_name[i] = '\0';
    }
    dev_name[39] = '\0';
	
	return 1;
}

int atapi_identify_device(int ide_num, int master, atapi_device_t *device) {
	int i = 0;
	
	int ide_ctrl = (ide_num == 1) ? ATA_PRIMARY_IDE : ATA_SECONDERY_IDE;
	
	// Drive spin-up
	outportb(ide_ctrl + 6, 0x00);
	
	// Send drive select.
	outportb(ide_ctrl + 6, (master) ? 0xA0 : 0xB0);
	for (i = 0; i < 5; i++)
		inportb(ide_ctrl + ATA_PORT_STATUS);
	
	uint8 status = inportb(ide_ctrl + ATA_PORT_STATUS);
	outportb(ide_ctrl + ATA_PORT_COMMAND, 0xEC);	// IDENTIFY command
	
	status = inportb(ide_ctrl + ATA_PORT_STATUS);
	
	if (status == 0)
		return 0;
	
	while (((status & ATA_STATUS_BSY) != 0) && ((status & ATA_STATUS_DRQ) == 0))
		status = inportb(ide_ctrl + ATA_PORT_STATUS);
	
	// Check for an AtapiPI device
	if(inportb(ide_ctrl + 2) == 0x01 &&
	   inportb(ide_ctrl + 3) == 0x01 &&
	   inportb(ide_ctrl + 4) == 0x14 &&
	   inportb(ide_ctrl + 5) == 0xEB)
	{
		outportb(ide_ctrl + ATA_PORT_COMMAND, 0xA1);
		status = inportb(ide_ctrl + ATA_PORT_STATUS);
		
		while ( ((status&0x80) != 0) && ((status&0x9) == 0) )
			status = inportb(ide_ctrl + ATA_PORT_STATUS);
	}
	else
	{
		return 0;
	}
	
	if (status & ATA_STATUS_ERR)
		return 0;
	
	for (i = 0; i < 256; i++)
	{
		buff[i] = inportw(ide_ctrl + ATA_PORT_DATA);
	}
	
	status = inportb(ide_ctrl + ATA_PORT_STATUS);
	
	for (i = 0; i < 20; i++)
	{
		device->name[i*2] = buff[0x1B+i] >> 8;
		device->name[i*2+1] = buff[0x1B+i] & 0xFF;
	}
	for (i = 39; i > 0; i--)
	{
		if (device->name[i] != ' ')
			break;
		device->name[i] = '\0';
	}
	device->name[39] = '\0';
	
	device->packet_size = ((buff[0] & 0x0003) == 0) ? 12 : 16;
	
	device->initialized = 1;
	device->cmd_reg = ide_ctrl;
	device->master = master;
	
//	uint32 bn, bs;
//	atapi_getCapacityInternal(&bn, &bs);
//	kprintf("Bn = %d, Bs = %d", bn, bs);
	
	return 1;
}

int atapi_getCapacityInternal(size_t *blockNumber, size_t *blockSize) {
	
	struct Capacity
	{
		uint32 LBA;
		uint32 BlockSize;
	} __attribute__((packed)) capacity;
	
	memset(&capacity, 0, sizeof(struct Capacity));
	
	struct CapacityCommand
	{
		uint8 Opcode;
		uint8 LunRelAddr;
		uint32 LBA;
		uint8 Rsvd[2];
		uint8 PMI;
		uint8 Control;
	} __attribute__((packed)) command;
	
	memset(&command, 0, sizeof(struct CapacityCommand));
	
	command.Opcode = 0x25; // READ CAPACITY
	int success = atapi_send_command(sizeof(struct Capacity), &capacity, sizeof(struct CapacityCommand), &command, 0);
	if(!success) {
		kprintf("ATAPI READ CAPACITY command failed\n");
		return 0;
	}
	
	*blockNumber = BIG_TO_HOST32(capacity.LBA);
	uint32 blockSz = BIG_TO_HOST32(capacity.BlockSize);
	*blockSize = blockSz ? blockSz : 2048;
	return 1;
}


int atapi_send_command(uint32 nRespBytes, void *respBuff, uint32 nPackBytes, void *packet, int bWrite) {
	int i = 0;
	
	if(!atapi_device.packet_size)
	{
		kprintf("sendCommand called but the packet size is not known!");
		return 0;
	}
	
	
	uint16 *tmpPacket = kmalloc(atapi_device.packet_size/2 * sizeof(uint16));//[m_PacketSize / 2];
	

	memcpy(tmpPacket, packet, nPackBytes);
	memset(tmpPacket + (nPackBytes / 2), 0, atapi_device.packet_size - nPackBytes);
	

	if(nPackBytes & 0x1)
		nPackBytes++;
	
	// PACKET command
	outportb(atapi_device.cmd_reg + 6, (atapi_device.master) ? 0xA0 : 0xB0);
	outportb(atapi_device.cmd_reg + 1, 0); // no overlap, no DMA
	outportb(atapi_device.cmd_reg + 2, 0); // tag = 0
	outportb(atapi_device.cmd_reg + 3, 0); // n/a for PACKET command
	outportb(atapi_device.cmd_reg + 4, (nRespBytes == 0) ? 0x2 : (nRespBytes & 0xFF)); // byte count limit
	outportb(atapi_device.cmd_reg + 5, ((nRespBytes >> 8) & 0xFF));
	outportb(atapi_device.cmd_reg + 7, 0xA0);
	
	
	uint8 status = inportb(atapi_device.cmd_reg + 7);
	while (((status & 0x80) != 0) && ((status & 0x9) == 0))
		status = inportb(atapi_device.cmd_reg + 7);
	
	
	if(status & 0x01) {
		kprintf("ATAPI Packet command error [status=0x%x]!", status);
		kfree(tmpPacket);
		return 0;
	}
	
	
	for(i = 0; i < (atapi_device.packet_size / 2); i++)
		outportw(atapi_device.cmd_reg + 0, tmpPacket[i]);
	

	status = inportb(atapi_device.cmd_reg + 7);
	while (((status & 0x80) != 0) && ((status & 0x9) == 0))
		status = inportb(atapi_device.cmd_reg + 7);
	
	if(status & 0x1)
	{
		kprintf("ATAPI sendCommand failed after sending command packet");
		kfree(tmpPacket);
		return 0;
	}
	
	
	if(!(status & 0x8))
		return 1;
	
	
	uint32 realSz = inportb(atapi_device.cmd_reg + 4) | (inportb(atapi_device.cmd_reg + 5) << 8);
	uint16 *dest = (uint16 *)respBuff;
	if(nRespBytes) {
		size_t sizeToRead = ((realSz > nRespBytes) ? nRespBytes : realSz) / 2;
		for(i = 0; i < sizeToRead; i++) {
			if(bWrite)
				outportw(atapi_device.cmd_reg + 0, dest[i]);
			else
				dest[i] = inportw(atapi_device.cmd_reg + 0);
		}
	}
	
	// Discard unread data (or write pretend data)
	if(realSz > nRespBytes)
	{
		kprintf("sendCommand has to read beyond provided buffer [%d is bigger than %d]", realSz, nRespBytes);
		for(i = nRespBytes; i < realSz; i += 2)
		{
			if(bWrite)
				outportw(atapi_device.cmd_reg + 0, 0xFFFF);
			else
				inportw(atapi_device.cmd_reg + 0);
		}
	}
	
	// Complete
	status = inportb(atapi_device.cmd_reg + 7);
	
	kfree(tmpPacket);
	return (!(status & 0x01));
}


void atapi_init() {
	char buff[40];
	
	kprintf("Detecting IDE drives...\n");
	
	//atapi_identify_device(2, 1, &atapi_device);
//	kprintf("Secondary IDE master: %s\n", atapi_device.name);
	
	if (!ata_identify_dev(1, 1, buff)) {
		
		if (!atapi_identify_device(1, 1, &atapi_device))
			strcpy(buff, "none");
		else
			strcpy(buff, atapi_device.name);
	}
	kprintf("Primary IDE master: %s\n", buff);
	
	if (!ata_identify_dev(1, 0, buff)) {
		if (!atapi_identify_device(1, 0, &atapi_device))
			strcpy(buff, "none");
		else
			strcpy(buff, atapi_device.name);
	}
	kprintf("Primary IDE slave: %s\n", buff);
	
	if (!ata_identify_dev(2, 1, buff)) {
		if (!atapi_identify_device(2, 1, &atapi_device))
			strcpy(buff, "none");
		else
			strcpy(buff, atapi_device.name);
	}
	kprintf("Secondary IDE master: %s\n", buff);
	
	if (!ata_identify_dev(2, 0, buff)) {
		if (!atapi_identify_device(2, 0, &atapi_device))
			strcpy(buff, "none");
		else
			strcpy(buff, atapi_device.name);
	}
	kprintf("Secondary IDE slave: %s\n", buff);
}
