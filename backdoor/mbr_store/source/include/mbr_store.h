/*
 * mbr_store.h
 * 
 * Copyright 2012 atzeton <atzeton@lavabit.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#ifndef HDR_MBR_STORE
#define HDR_MBR_STORE

#include <nettle/sha.h>

#define VERSION 		"1.0"

#define __EXIT_SUCCESS  exit(EXIT_SUCCESS)
#define __EXIT_FAILURE  exit(EXIT_FAILURE)

#define __STORE 		(mode == MODE_STORE)
#define __READ 			(mode == MODE_READ)

#define __ENCRYPTED		encrypted

#define __RAND 			(uint8_t) rand() % 255

#define CRC32POLY 		(0x04C11DB7)

#define GRUB_INDICATION "GRUB"

#define DATA_SIZE 		(428)
#define BLOCK_SIZE 		(432)
#define MBR_SIZE 		(512)

#define MODE_STORE 		(  1)
#define MODE_READ 		(  2)


uint8_t 	mode;
char       *device;
char 	   *password;
int8_t 		encrypted;
uint8_t 	key[SHA256_DIGEST_SIZE];


/* MBR structure */
typedef struct mbr_struct {
	uint8_t data[440];
	uint32_t disk_sig;
	uint16_t nulls;
	uint8_t ptable[64];
	uint16_t signature;
} __attribute__ ((packed)) mbr_struct;


/* data storage structure */
typedef struct data_header {
	uint16_t len;
	uint32_t crc;
	uint8_t data[DATA_SIZE];
	uint8_t payload[8];
} __attribute__ ((packed)) data_header;


int main(int argc, char **argv);

#endif
